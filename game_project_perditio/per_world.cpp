#include "stdafx.h"
#include "per_world.h"
#include "per_controller.h"
#include "per_renderer.h"
#include "per_game.h"
#include "per_object.h"
#include "object_storage.h"
#include "game_mode.h"
#include "per_hud.h"
#include "event_dispatcher.h"
#include "black_board.h"
#include "per_component.h"
#include "stuck_physics_component.h"
#include "spawner_ai_component.h"
#include "per_particle_pool.h"
#include "graphics_component.h"

PERWorld::PERWorld()
{
	m_particlePool = new PERParticlePool();
}

PERWorld::~PERWorld()
{
	PERLog::Logger().Info("���� ����");
	delete m_particlePool;
}

void PERWorld::Update(PERAudio& audio, double dTime)
{
	DoGarbegeCollection(dTime);

	ProcessPendingMessage();

	m_gameMode->Update();
}

void PERWorld::UIUpdate(PERController& controller, PERAudio& audio, double dTime)
{
	m_gameMode->GetHud().Update(controller, audio, dTime);
}

void PERWorld::InputUpdate(PERController& controller, PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_inputComponents[i]->Update(*this, controller, audio, dTime);
	}
}

void PERWorld::AiUpdate(PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_aiComponents[i]->Update(*this, audio, dTime);
	}
}

void PERWorld::PhysicsUpdate(PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_physicsComponents[i]->Update(*this, audio, dTime);
	}
	m_particlePool->Update(dTime);
}

void PERWorld::GraphicsUpdate(PERAudio& audio, double dTime)
{
	for (int i = 0; i < m_numObject; ++i) 
	{
		m_graphicsComponents[i]->Update(GetHud(), audio, dTime);
	}
}

void PERWorld::Render(PERRenderer& renderer, double frameGap)
{
	for (size_t i = 0; i < m_sortedGraphicsComponents.size(); ++i) {
		m_sortedGraphicsComponents[i]->Render(renderer, frameGap);
	}
	m_particlePool->Renderer(renderer);
	GetHud().RendererInWorld(renderer, *m_database);
}

void PERWorld::UIRender(PERRenderer& renderer)
{
	GetHud().Renderer(renderer, *m_database);
}

void PERWorld::Enter()
{
	AddObject(&m_gameMode->GetPlayer());

	InitWorldObject();

	m_gameMode->StartUse();
}

void PERWorld::Exit()
{
	m_gameMode->EndUse();
}

void PERWorld::Pause()
{
}

void PERWorld::Resume()
{
}

void PERWorld::RequestAddObject(PERObject* parent, PERObjectType type, const char* databaseId, bool isVisualId, PERStat stat,
	PERVec3 position, double lifeTime, PERVec3 currentAccel)
{
	if (m_maxPending == m_numPending) ResizePedingArray();

	m_pending[m_numPending].id = PERWorldMessageId::ADD_OBJECT;
	m_pending[m_numPending].object = parent;
	m_pending[m_numPending].type = type;
	m_pending[m_numPending].databaseId = databaseId;
	m_pending[m_numPending].isVisualId = isVisualId;
	m_pending[m_numPending].position = position;
	m_pending[m_numPending].currentAccel = currentAccel;
	m_pending[m_numPending].lifeTime = lifeTime;
	m_pending[m_numPending].stat = stat;

	m_numPending++;
}

void PERWorld::RequestDeleteObject(PERObject* object)
{
	if (m_maxPending == m_numPending) ResizePedingArray();

	m_pending[m_numPending].id = PERWorldMessageId::DELETE_OBJECT;
	m_pending[m_numPending].object = object;
	m_numPending++;
}

bool PERWorld::CheckCollision(PERObject& object, double dTime)
{
	bool collided = false;
	bool isOnPlatform = false;

	// ���� ������Ʈ ���� ���
	PERObjectType type = object.GetObjectType();
	PERVec3 position = object.GetBoundingBoxPosition(), size = object.GetBoundingBoxSize();
	PERBoundingType boundingtype = object.GetBoundingType();

	int id = object.GetIDInWorld();
	for (int i = 0; i < m_numObject; ++i) {
		// ������ �׾��ų� ���� ������Ʈ �ǳʶ�
		if (object.GetLifeTime() < 0.0) return collided;
		if (m_objects[i]->GetLifeTime() < 0.0) continue;

		// id�� ���� ��(����), �θ�, �ڽ� �ǳʶ�
		if (id == i) continue;
		if (object.GetParent() == m_objects[i]) continue;
		if (&object == (m_objects[i]->GetParent())) continue;

		// ����� z���� ������ ������ �ǳʶ�
		if ((int)(floor(m_objects[i]->GetPosition().z)) > (int)floor(position.z)) continue;
		// ����� z�� + 1�� ������ ������ �ǳʶ�
		if ((int)(floor(m_objects[i]->GetPosition().z)) + 1 < (int)floor(position.z)) continue;

		// ���� �浹�� ������Ʈ�� ���� ��� �浹 ������Ʈ ����
		if (object.GetCollidedObject() == m_objects[i]) object.SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));
		if (&object == m_objects[i]->GetCollidedObject()) m_objects[i]->SetCollidedObject(nullptr, PERVec3(0.0, 0.0, 0.0));

		// �浹 ó�� ���� -> type(�浹 �˻縦 ��û�� ������Ʈ�� Ÿ��)�� fixed object, trigger�� �� �� ����
		// 
		// movable object && movable object == ���� ����
		// movable object && trigger == ���� ����
		// bullet(blade) && trigger == ���� ����
		// 
		// movable object && fixed object == fixed ������Ʈ�� ������(���� �� �� ����)
		// movable object && (player || monster) == ������ ó��
		// (player || monster) && fixed object  == ������ ó��
		// 
		// ������ ���� == ������ ó�� ���� �浹 ó��

		// ��� ������Ʈ Ÿ�� ���
		PERObjectType otherType = m_objects[i]->GetObjectType();
		PERVec3 otherPos = m_objects[i]->GetBoundingBoxPosition(), otherSize = m_objects[i]->GetBoundingBoxSize();
		PERBoundingType otherBoundingType = m_objects[i]->GetBoundingType();

		// Ư�� Ÿ�� ó��
		// ���� ���� ������ ó��
		if (otherType == PERObjectType::DOOR) otherType = PERObjectType::FIXED_BLOCK;
		else if (otherType == PERObjectType::BUTTON) continue;
		else if (otherType == PERObjectType::PARTICLE_EFFECTER) continue;

		// �浹 ó�� ���� �׸�
		if (type == PERObjectType::MOVABLE_BLOCK && otherType == PERObjectType::MOVABLE_BLOCK) continue;
		else if ((type == PERObjectType::BULLET || type == PERObjectType::BLADE) && otherType == PERObjectType::PRESSURE) continue;
		else if ((otherType == PERObjectType::BULLET || otherType == PERObjectType::BLADE) && type == PERObjectType::PRESSURE) continue;

		if (otherBoundingType == PERBoundingType::RECTANGLE && boundingtype == PERBoundingType::RECTANGLE) {
			if (CheckAABBCollision(position, size, otherPos, otherSize)) {
				// ��밡 ������ �ٷ� �Ʒ� �ְ� ���� ���� ��� �÷��� ���� �ִٰ� ���� �� �ǳʶ�(��)
				if ((int)(floor(otherPos.z)) == (int)floor(position.z) -1 && otherType == PERObjectType::FIXED_BLOCK) {
					isOnPlatform = true;  
					continue;
				}
				// �浹��ٰ� ����
				collided = true;

				// ������ ó�� �׸�
				if (type == PERObjectType::MOVABLE_BLOCK && otherType == PERObjectType::FIXED_BLOCK)
					AdjustPositionWithObjects(object, position, size, *m_objects[i], otherPos, otherSize, dTime);
				else if (type == PERObjectType::MOVABLE_BLOCK && (otherType == PERObjectType::PLAYER || otherType == PERObjectType::MONSTER))
					AdjustPositionWithObjects(object, position, size, *m_objects[i], otherPos, otherSize, dTime);
				else if ((type == PERObjectType::PLAYER || type == PERObjectType::MONSTER) && otherType == PERObjectType::MOVABLE_BLOCK)
					AdjustPositionWithObjects(object, position, size, *m_objects[i], otherPos, otherSize, dTime);
				else if ((type == PERObjectType::PLAYER || type == PERObjectType::MONSTER) && otherType == PERObjectType::FIXED_BLOCK)
					AdjustPositionWithObjects(object, position, size, *m_objects[i], otherPos, otherSize, dTime);

				// ������ ó�� ���� �ٸ� ��ȣ �ۿ�
				else ProcessCollisionWithoutMoving(object, type, *m_objects[i], otherType, dTime);

			}
		}
	}

	// �÷��� ���� ���� ���� ��� ������ ����
	if (!isOnPlatform) {
		// �ӽ÷� ������ Ÿ���� 0���� �۵��� ������ �ܼ��� ������Ʈ ����
		object.SetLifeTime(-1.0);
	}

	return collided;
}

PERHud& PERWorld::GetHud()
{
	return m_gameMode->GetHud();
}

PERDatabase& PERWorld::GetDatabase()
{
	return *m_database;
}

PERParticlePool& PERWorld::GetParticlePool()
{
	return *m_particlePool;
}

void PERWorld::DoGarbegeCollection(double dTime)
{
	for (int i = 0; i < m_numObject; ++i) {
		if (m_objects[i]->IsLifeTimeEnd(dTime)) {
			RequestDeleteObject(m_objects[i]);
		}
	}
}

void PERWorld::ProcessPendingMessage()
{
	for (int i = 0; i < m_numPending; ++i) {
		PERWorldMessage message = m_pending[i];

		switch (message.id) {
		case PERWorldMessageId::ADD_OBJECT: {
			PERObject* newObject = AddAndGetObject(message.type);

			// databaseId�� ���־� ���̵� �ƴ� ��� ���� ���̵�(������Ʈ ���̵�)�̹Ƿ� �����ͺ��̽����� ���־� ���̵� ����
			if (!message.isVisualId) {
				newObject->GetObjectState().SetNameId(m_database->GetMonsterData(message.databaseId.c_str())->nameId);
				message.databaseId = m_database->GetMonsterData(message.databaseId.c_str())->visualId;
			}

			VisualData* vData = m_database->GetVisualData(message.databaseId.c_str());
			PERComponent::GraphicsData gData;
			gData.shape = vData->shape; gData.color = vData->color;
			gData.border = vData->borderOn; gData.borderWidth = vData->borderWidth; gData.borderColor = vData->borderColor;
			newObject->SetSize(vData->size);
			newObject->GetGraphics().SetData(gData);

			newObject->SetPosition(message.position);
			newObject->SetCurrentAccel(message.currentAccel);
			newObject->SetLifeTime(message.lifeTime);
			newObject->SetParent(message.object);
			newObject->GetObjectState().SetStat(message.stat);

			newObject->SetCurrentPositionToSpawnPosition();

			// Į���� ��� ��ġ���� �ٿ��� ��ġ�� ����
			if (message.type == PERObjectType::BLADE)
			{
				PERComponent::PhysicsData pData;
				pData.stuckPosition = message.position;
				newObject->GetPhysics().SetData(pData);
			}

			// ���ο� ������Ʈ �߰��� ��û�� ������Ʈ�� �������� ��� �������� ai ������Ʈ�� �߰��� ������Ʈ ����
			if (message.object->GetObjectType() == PERObjectType::SPAWNER)
			{
				dynamic_cast<SpawnerAiComponent*>(&message.object->GetAi())->SetSpawnedObject(newObject);
			}

			break;
		}
		case PERWorldMessageId::DELETE_OBJECT: 
			DeleteObject(message.object);
			break;
		}
	}
	m_numPending = 0;
}

void PERWorld::AddObject(PERObject* object)
{
	if (m_objects.size() == m_numObject) {
		m_maxObject *= 2;
		m_objects.reserve(m_maxObject);
		m_objects.resize(m_maxObject);
		// ������Ʈ�鵵 ��������
		m_inputComponents.resize(m_maxObject);
		m_aiComponents.resize(m_maxObject);
		m_physicsComponents.resize(m_maxObject);
		m_graphicsComponents.resize(m_maxObject);
	}
	m_inputComponents[m_numObject]		= &object->GetInput();
	m_aiComponents[m_numObject]			= &object->GetAi();
	m_physicsComponents[m_numObject]	= &object->GetPhysics();
	m_graphicsComponents[m_numObject]	= &object->GetGraphics();

	m_objects[m_numObject] = object;
	object->SetIDInWorld(m_numObject);
	m_numObject++;

	m_isUpdateSortedGraphicsComponent = false;
}

void PERWorld::DeleteObject(PERObject* object)
{
	m_numObject--;
	int id = object->GetIDInWorld();
	// �� �� ������Ʈ �̵�
	m_inputComponents[id]		= m_inputComponents[m_numObject];
	m_aiComponents[id]			= m_aiComponents[m_numObject];
	m_physicsComponents[id]		= m_physicsComponents[m_numObject];
	m_graphicsComponents[id]	= m_graphicsComponents[m_numObject];
	// �� �� ������Ʈ �̵� �� ���̵� ����
	m_objects[id] = m_objects[m_numObject];
	m_objects[id]->SetIDInWorld(id);
	m_objectStorage->PushObject(object->GetObjectType(), object);

	m_isUpdateSortedGraphicsComponent = false;
}

PERObject* PERWorld::AddAndGetObject(PERObjectType type)
{
	PERObject* object = m_objectStorage->PopObject(type);
	AddObject(object);
	return object;
}

void PERWorld::InitWorldObject()
{
	// �׺���̼� �����Ϳ� ������ �ִ� ������Ʈ ���� �߰�
	AddFixedAndPhysicalObjects();

	// �׺���̼� ������ ����
	BlackBoard::GetNavigationData().InitCells();
	BlackBoard::GetNavigationData().SetCells(m_objects, m_numObject);

	// �׺���̼� �����Ϳ� ������� ������ ������Ʈ �߰�
	AddOtherObjects();

	PERLog::Logger().InfoWithFormat("���� �� ������Ʈ ��: %d", m_numObject);
}

void PERWorld::InitSettingForWorld(ObjectStorage* objectStorage, PERDatabase* database, GameMode* mode)
{
	PERLog::Logger().Info("���� ����");

	m_objectStorage = objectStorage;
	m_database = database;
	m_gameMode = mode;

	// ������Ʈ, ������Ʈ ���� ��������
	m_objects.reserve(m_maxObject);
	m_objects.resize(m_maxObject);
	m_inputComponents.resize(m_maxObject);
	m_aiComponents.resize(m_maxObject);
	m_physicsComponents.resize(m_maxObject);
	m_graphicsComponents.resize(m_maxObject);
}

void PERWorld::SetObjectVisual(PERObject* object, const char* visualId)
{
	VisualData* data = m_database->GetVisualData(visualId);

	object->SetSize(data->size);
	object->SetMass(data->mass);
	object->SetBoundingType(data->boundingType);
	SetObjectShapeAndColor(object, data->shape, data->color, data->borderOn, data->borderWidth, data->borderColor);
}

void PERWorld::SetObjectShapeAndColor(PERObject* object, PERShapeType shape, PERColor color,
	bool border, int borderWidth, PERColor borderColor)
{
	PERComponent::GraphicsData graphicsData;
	graphicsData.shape = shape; graphicsData.color = color;
	graphicsData.border = border; graphicsData.borderWidth = borderWidth; graphicsData.borderColor = borderColor;

	object->GetGraphics().SetData(graphicsData);
}

void PERWorld::UpdateSortedGraphicsComponents()
{
	if (m_isUpdateSortedGraphicsComponent) return;

	m_sortedGraphicsComponents.clear();

	// ��ġ�� z���� ���� ������ ����
	m_sortedGraphicsComponents.resize((size_t)m_numObject);
	std::copy(m_graphicsComponents.begin(), m_graphicsComponents.begin() + m_numObject, m_sortedGraphicsComponents.begin());
	std::sort(m_sortedGraphicsComponents.begin(), m_sortedGraphicsComponents.end(), [](GraphicsComponent* a, GraphicsComponent* b) {
		return a->GetZValue() < b->GetZValue();
		});

	m_isUpdateSortedGraphicsComponent = true;
}

void PERWorld::UpdateCamera(PERRenderer& renderer, double frameGap)
{
	// ���� ��忡 ���� ī�޶� ������Ʈ
	m_gameMode->UpdateCamera(renderer, frameGap);
}

void PERWorld::ResizePedingArray()
{
	// ���� �迭 ũ�⸦ 2��� ����
	m_maxPending *= 2;
	PERWorldMessage* newArray = new PERWorldMessage[m_maxPending];
	memmove(newArray, m_pending, sizeof(m_pending));

	delete[] m_pending;
	m_pending = newArray;
}

bool PERWorld::CheckAABBCollision(PERVec3 aPos, PERVec3 aSize, PERVec3 bPos, PERVec3 bSize)
{
	PERVec3 aHalfSize = aSize * 0.5;
	PERVec3 bHalfSize = bSize * 0.5;

	if (aPos.x - aHalfSize.x > bPos.x + bHalfSize.x) return false;
	if (aPos.x + aHalfSize.x < bPos.x - bHalfSize.x) return false;
	if (aPos.y - aHalfSize.y > bPos.y + bHalfSize.y) return false;
	if (aPos.y + aHalfSize.y < bPos.y - bHalfSize.y) return false;
	
	return true;
}

void PERWorld::AdjustPositionWithObjects(PERObject& aObject, PERVec3 aPos, PERVec3 aSize,
	PERObject& bObject, PERVec3 bPos, PERVec3 bSize, double dTime)
{
	// ���� �浹�� ���� ������ ����� �� ���
	PERVec3	aVel = aObject.GetCollidedVelocity(); double aMass = aObject.GetCollidedMass();
	PERVec3 bVel = bObject.GetCollidedVelocity(); double bMass = bObject.GetCollidedMass();

	// �浹 ó��(���ſ� ���� ������ �ɷ� ����)
	if (aMass > bMass) {
		ProcessCollisionBetweenFixedAndMovable(
			aObject, aPos, aSize, aVel,
			bObject, bPos, bSize, bVel, dTime);
	}
	else if (aMass <= bMass) {
		ProcessCollisionBetweenFixedAndMovable(
			bObject, bPos, bSize, bVel, 
			aObject, aPos, aSize, aVel, dTime);
	}
}

void PERWorld::ProcessCollisionBetweenFixedAndMovable(
	PERObject& fixedObject, PERVec3 fixedPos, PERVec3 fixedSize, PERVec3 fixedVel,
	PERObject& movableObject, PERVec3 movablePos, PERVec3 movableSize, PERVec3 movableVel, double dTime)
{
	// �浹 �ӵ� ���
	PERVec3 collisionVelocity = movableVel - fixedVel;

	// �浹 �Ÿ� ���
	PERVec3 fixedHalfSize = fixedSize * 0.5;
	PERVec3 movableHalfSize = movableSize * 0.5;

	PERVec3 collisionDistance = PERVec3(0.0, 0.0, 0.0);
	PERVec3 collisionTimeRate = PERVec3(0.0, 0.0, 0.0);

	if (collisionVelocity.x > 0.0) collisionDistance.x = (movablePos.x + movableHalfSize.x) - (fixedPos.x - fixedHalfSize.x);
	else if (collisionVelocity.x < 0.0) collisionDistance.x = (fixedPos.x + fixedHalfSize.x) - (movablePos.x - movableHalfSize.x);

	if (collisionVelocity.y > 0.0) collisionDistance.y = (movablePos.y + movableHalfSize.y) - (fixedPos.y - fixedHalfSize.y);
	else if (collisionVelocity.y < 0.0) collisionDistance.y = (fixedPos.y + fixedHalfSize.y) - (movablePos.y - movableHalfSize.y);

	if (collisionVelocity.x == 0.0) {
		if (collisionDistance.x == 0.0) collisionTimeRate.x = 0.0;
		else collisionTimeRate.x = 1.0;
	}
	else collisionTimeRate.x = std::abs(collisionDistance.x / collisionVelocity.x);

	if (collisionVelocity.y == 0.0) {
		if (collisionDistance.y == 0.0) collisionTimeRate.y = 0.0;
		else collisionTimeRate.y = 1.0;
	}
	else collisionTimeRate.y = std::abs(collisionDistance.y / collisionVelocity.y);

	double collisionTime = collisionTimeRate.x * dTime;
	if (collisionTimeRate.x < collisionTimeRate.y) collisionTime = collisionTimeRate.y * dTime;

	// �� ������Ʈ���� �浹 ó��
	fixedObject.GetPhysics().ProcessCollision(movableObject, collisionVelocity, fixedVel, 0.0);
	movableObject.GetPhysics().ProcessCollision(fixedObject, collisionVelocity, PERVec3(0.0, 0.0, movableVel.z), dTime * 1.5);
}

void PERWorld::ProcessCollisionWithoutMoving(PERObject& aObject, PERObjectType aType, PERObject& bObject, PERObjectType bType, double dTime)
{
	// bullet(blade) && bullet(blade) == ������ ó���� ����, bullet(blade) ����
	// bullet(blade) && fixed object == ������ ó���� ����, bullet(blade) ����
	// bullet(blade) && movable object == ������ ó���� ����, bullet(blade) ����
	// bullet(blade) && (player || monster) == ������ ó���� ����, bullet(blade) ����, ������ ó��
	// bullet(blade) && trigger == ���� ����
	// (player || monster) && (player || monster) == ������ ó���� ����, ������ ó��		 
	// (player || monster) && trigger == ������ ó���� ����

	// �Ѿ��̶� Į���� ó�� ����� �����ϱ� ������ Į���� �Ѿ˷� �����
	if (aType == PERObjectType::BLADE) aType = PERObjectType::BULLET;
	if (bType == PERObjectType::BLADE) bType = PERObjectType::BULLET;

	// �Ѿ˰� ���
	if (aType == PERObjectType::BULLET && bType == PERObjectType::BULLET) {
		aObject.SetLifeTime(-1.0);
		bObject.SetLifeTime(-1.0);
	}
	// �Ѿ� ������ ó��
	else if (aType == PERObjectType::BULLET) {
		bObject.GetObjectState().GiveDamage(aObject, aObject.GetObjectState().GetStat().physicalAttack, aObject.GetObjectState().GetStat().mindAttack);

		// �Ѿ� �ӵ� �������� �ణ �̵�(�˹�)
		if (aObject.GetObjectType() == PERObjectType::BULLET) {
			aObject.SetLifeTime(-1.0);
			bObject.GetPhysics().GiveForce(*this, NormalizeVector(aObject.GetVelocity()) * PER_KNOCK_BACK_POWER, dTime);
		}
		// Į���� ����� ��ġ �������� �ణ �̵�(�˹�)
		else if (aObject.GetObjectType() == PERObjectType::BLADE) {
			StuckPhysicsComponent& stuckPhysics = dynamic_cast<StuckPhysicsComponent&>(aObject.GetPhysics());
			bObject.GetPhysics().GiveForce(*this, NormalizeVector(stuckPhysics.GetStuckPosition()) * PER_KNOCK_BACK_POWER, dTime);
		}
	}
	else if (bType == PERObjectType::BULLET) {
		aObject.GetObjectState().GiveDamage(bObject, bObject.GetObjectState().GetStat().physicalAttack, bObject.GetObjectState().GetStat().mindAttack);

		// �Ѿ� �ӵ� �������� �ణ �̵�(�˹�)
		if (bObject.GetObjectType() == PERObjectType::BULLET) {
			bObject.SetLifeTime(-1.0);
			aObject.GetPhysics().GiveForce(*this, NormalizeVector(bObject.GetVelocity()) * PER_KNOCK_BACK_POWER, dTime);
		}
		// Į���� ����� ��ġ �������� �ణ �̵�(�˹�)
		else if (aObject.GetObjectType() == PERObjectType::BLADE) {
			StuckPhysicsComponent& stuckPhysics = dynamic_cast<StuckPhysicsComponent&>(bObject.GetPhysics());
			aObject.GetPhysics().GiveForce(*this, NormalizeVector(stuckPhysics.GetStuckPosition()) * PER_KNOCK_BACK_POWER, dTime);
		}
	}
	// ������
	// �÷��̾�� ���� ��
	else if (aType == PERObjectType::PLAYER && bType == PERObjectType::MONSTER) {
		// �÷��̾�� ������� ��
		aObject.GetObjectState().GiveDamage(bObject, bObject.GetObjectState().GetCollisionDamage(), 0);
	}
	else if (bType == PERObjectType::PLAYER && aType == PERObjectType::MONSTER) {
		bObject.GetObjectState().GiveDamage(aObject, aObject.GetObjectState().GetCollisionDamage(), 0);
	}
	// �÷��̾� �Ǵ� ���� �Ǵ� �����̴� ������Ʈ �� �з� ���� ��
	else if ((aType == PERObjectType::PLAYER || aType == PERObjectType::MONSTER || aType == PERObjectType::MOVABLE_BLOCK) && bType == PERObjectType::PRESSURE) {
		// �浹 ó��(�ʿ��� �� ���� ������Ʈ ��, ���������� ������ ���� �Էµ� ������ ó����)
		bObject.GetPhysics().ProcessCollision(aObject, PERVec3(0.0, 0.0, 0.0), PERVec3(0.0, 0.0, 0.0), dTime);
	}
	else if ((bType == PERObjectType::PLAYER || bType == PERObjectType::MONSTER || bType == PERObjectType::MOVABLE_BLOCK) && aType == PERObjectType::PRESSURE) {
		aObject.GetPhysics().ProcessCollision(bObject, PERVec3(0.0, 0.0, 0.0), PERVec3(0.0, 0.0, 0.0), dTime);
	}
}