#pragma once
#define PER_DEBUG

// 투명 관련 라이브러리
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "irrKlang.lib")

// 윈도우즈 API 
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
// 멀티 스레드
#include <thread>
#include <mutex>
#include <atomic>
#include <ppl.h>
#include <concurrent_queue.h>
// 콘솔 입출력
#include <iostream>
#include <string>
#include <stdarg.h>
// 파일 입출력
#include <fstream>
// 프레임 타임 측정
#include <chrono>
// 편의성
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <functional>

// 디버깅
#include <cassert>

// 필요 구조체
#include "per_vector.h"
#include "per_data.h"

// json
#include "./rapidjson/document.h"
#include "./rapidjson/filereadstream.h"

// sound
#include "./irrKlang/irrKlang.h"

// 로그
#include "per_log.h"

// 기본 설정
// 윈도우 창 크기 및 위치
#define PER_DEFAULT_WINDOW_LOCATION_X 200
#define PER_DEFAULT_WINDOW_LOCATION_Y 100
#define PER_DEFAULT_WINDOW_WIDTH 1280
#define PER_DEFAULT_WINDOW_HEIGHT 720
#define PER_DEFAULT_WINDOW_BORDER_WIDTH 16
#define PER_DEFAULT_WINDOW_BAR_HEIGHT 39
#define PER_MAXIMUM_WINDOW_WIDTH 1800
#define PER_MAXIMUM_WINDOW_HEIGHT 1000

// 스레드
#define PER_NUM_WORKER_THREAD 7

// 투명 레이어
#define PER_TRANSPARENT_COLOR RGB(1, 12, 123)

// 업데이트 타임 설정
#define PER_MICROSEC_PER_UPDATE 8000
#define PER_MAXIMUM_UPDATE_LOOP_COUNT 4
// 최소 프레임 타임(최대 240FPS)
#define PER_MINIMUM_FRAME_TIME 4000

// 화면 설정
#define PER_PIXEL_PER_METER 100
#define PER_DEFAULT_CAMERA_SIGHT_WIDTH 20.0
// 컨트롤러 설정
#define PER_KEYBOARD_LONG_PRESS_TIME 0.5

// 물리 값
#define PER_GRAVITY 9.8
#define PER_FRICTION 0.7
#define PER_KNOCK_BACK_POWER 50000.0
// 가장 낮은 z값(높이 값)
#define PER_LOWEST_Z_VALUE -10000.0

// 수학 값
#define PER_PI 3.141592
#define ONE_DEGREE_IN_RADIAN 0.0174533

// 객체
#define PER_DEFAULT_OBJECT_POOL_SIZE  1024
#define PER_DEFAULT_MAX_OBJECTS		  1024

// 파티클
#define PER_DEFAULT_MAX_PARTICLES	  4096

// UI
#define PER_DEFAULT_UI_ELEMENT_POOL_SIZE 256
#define PER_MAXIMUM_TEXT_SIZE 32

// 이벤트
#define PER_DEFAULT_MAX_EVENT_PENDING  256

// 크기별 슬립 거리(업데이트 및 렌더링을 하지 않는 거리) 설정
#define PER_SMALL_OBJECT_SLEEP_DISTANCE 300
#define PER_BIG_OBJECT_SLEEP_DISTANCE 500
#define PER_BULLET_OBJECT_SLEEP_DISTANCE 100'000'000
#define PER_SMALL_OBJECT_SIZE_LIMIT 5.0

// 네비게이션 데이터
#define PER_NON_CELL			0
#define PER_CELL_DATA_WEIGHT	5
#define PER_CELL_DISTANCE		0.25
#define PER_MAX_CELL			100

// 시간
// 최대 라이프 타임(100년)
#define PER_MAXIMUM_LIFE_TIME 3'153'600'000.0

// 게임 
#define PER_MAX_STAT 9999


namespace std {
	int clamp(int value, int min, int max);
}