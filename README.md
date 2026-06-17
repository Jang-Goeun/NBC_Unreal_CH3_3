# 🎮NBC_Unreal_CH3_3: NBC Unreal Unreal 3D Action Game Project

CH3의 과제이며, 언리얼 엔진 5를 활용한 3D 액션 아이템 수집 게임입니다. 다양한 아이템을 수집하며 제한 시간 내에 웨이브를 클리어하고, 여러
레벨을 돌파하는 게임 루프를 포함하고 있습니다.

<br>

## 🚀 주요 기능 (Key Features)

### 1. 멀티 웨이브 및 레벨 시스템 (Multi-Wave & Level System)
* 3단계 레벨 전환: Basic, Intermediate, Advanced 총 3개의 레벨로 구성
* 레벨 내 멀티 웨이브: 각 레벨 이동 없이 내부적으로 3회의 웨이브가 진행됩니다.
* 동적 난이도 조절: 웨이브가 진행될수록 제한 시간이 짧아지고 스폰되는 아이템 개수가 늘어나 긴장감을 유도합니다.
* 실시간 알림: 웨이브 시작 시 화면 메시지 및 출력 로그를 통해 현재 진행 상태를 플레이어에게 직관적으로 전달합니다.

### 2. UI/UX 디자인
* 통합 HUD: 점수, 남은 시간, 플레이어 체력(HP Bar)을 한 화면에서 실시간으로 확인 가능합니다.
* 인터랙티브 메뉴:
   * 메인 메뉴: 시작(Start), 게임 종료(Quit), 메인 메뉴로 이동 기능 제공.
   * 게임 오버 메뉴: 재시작(Restart), 게임 종료(Quit), 메인 메뉴로 이동 기능 제공.
* 디자인 업그레이드: Vertical Box와 Padding을 활용한 정돈된 레이아웃, 반투명 배경 및 폰트 설정을 통한 시각적 가독성 향상.

### 3. 아이템 및 상호작용 시스템
* 다양한 아이템: Coin(점수), Healing(체력 회복), Mine(대미지 및 폭발) 등 인터페이스 기반의 아이템 시스템.
* 확률 기반 스폰: SpawnVolume과 데이터 테이블을 연동하여 전략적인 아이템 배치.

<br>

## 🛠 기술적 구현 세부사항 (Technical Implementation)

### 아키텍처 (Architecture)
* Ch3GameState: 전체 게임 흐름(웨이브 제어, 타이머 관리, HUD 업데이트)의 중심 로직 구현.
* Ch3PlayerController: 입력 처리와 위젯 생성/제거를 통한 UI 상태 전환 관리.
* Ch3GameInstance: 레벨 전환 시에도 누적 점수와 현재 레벨 인덱스를 유지하는 데이터 영속성 처리.

### 안정성 및 최적화 (Stability & Safety)
* 크래시 방지 로직: IsValid() 체크와 WeakObjectPtr 개념을 적용하여 레벨 전환 시 발생할 수 있는 메모리 참조 오류(Access
 Violation) 해결.
* 타이머 관리: 레벨 종료 및 전환 시 모든 잔여 타이머를 명시적으로 정리(ClearTimer)하여 좀비 프로세스 및 런타임 에러 방지.
* C++ & Blueprint: 핵심 로직(레벨 이동, 게임 종료 등)은 C++로 구현하고, 시각적 디자인과 버튼 이벤트 바인딩은
 블루프린트를 활용하여 유연성 확보.

<br>

## 🕹 조작 방법 (Controls)
* Move: W, A, S, D
* Jump: Space Bar
* Look: Mouse
* Sprint: Left Shift

<br>

## 시연 영상
https://github.com/user-attachments/assets/341a5959-e5eb-46db-9359-cf27b15daad4
