# # ⚾ Multiplayer Bulls and Cows (숫자 야구 게임)

언리얼 엔진 5를 활용하여 제작된 **네트워크 멀티플레이어 숫자 야구 게임**입니다. 클라이언트-서버 구조에서 데이터 동기화(Replication)와 RPC(Remote Procedure Call)를 통해 실시간 채팅 판정 및 UI 업데이트를 구현하였습니다.

---

### ### 🛠 핵심 기술 스택
* **Engine:** Unreal Engine 5.x
* **Language:** C++
* **Networking:** Listen Server 구조, Property Replication (`OnRep`), RPC (`Server`/`Client`/`Multicast`)
* **UI:** UMG (Unreal Motion Graphics) & **C++ Data Binding**

---

### ### 🏗 프로젝트 아키텍처 (Core Classes)

| 클래스 | 주요 역할 |
| :--- | :--- |
| **`AynGameModeBase`** | **Authority 로직:** 정답 번호 생성, 입력값 판정(Strike/Ball), 승리 조건 체크 및 게임 리셋 관리. |
| **`AynPlayerController`** | **통신 인터페이스:** 로컬 입력(ServerRPC) 전달, 위젯 생성 및 `NotificationText` 변수 동기화(`OnRep`). |
| **`AynPlayerState`** | **데이터 보관함:** 플레이어 이름, 현재 시도 횟수(`CurrentGuessCount`), 최대 기회 등의 정보를 네트워크에 공유. |
| **`AynGameStateBase`** | **글로벌 상태:** 모든 플레이어에게 로그인 메시지 등을 브로드캐스팅(`MulticastRPC`). |
| **`AynPawn`** | 게임 내 플레이어의 물리적 존재를 담당하며, 네트워크 역할(Role) 확인용 디버깅 로그 출력. |

---

### ### 🚀 주요 시스템 및 로직 설명

#### 1. 네트워크 입력 및 판정 흐름
* **Input:** 클라이언트가 채팅창에 숫자를 입력하면 `AynPlayerController`에서 `ServerRPCPrintChatMessageString`을 호출하여 서버로 메시지를 보냅니다.
* **Judge:** 서버의 `GameMode`는 `SecretNumberString`과 입력값을 비교하여 스트라이크/볼 개수를 계산합니다.
* **Result:** 판정 결과는 다시 모든 클라이언트의 채팅창에 출력됩니다.

#### 2. 효율적인 UI 업데이트 (ReplicatedUsing)
* 서버에서 승리자가 발생하면 `AynPlayerController`의 `NotificationText` 변수를 갱신합니다.
* **`UPROPERTY(ReplicatedUsing = OnRep_NotificationText)`** 설정을 통해 값이 변하는 즉시 클라이언트의 `OnRep` 함수가 트리거됩니다.
* 위젯 블루프린트에서 **Text 변수를 C++의 `NotificationText`와 직접 바인딩(Binding)** 하여, 별도의 호출 로직 없이도 데이터가 바뀌면 화면이 즉시 갱신됩니다.



#### 3. 자동 게임 리셋 시스템
* 3스트라이크(승리) 또는 모든 기회 소진(무승부) 시, 서버는 `FTimerHandle`을 통해 3초의 유예 시간을 가집니다.
* 모든 플레이어에게 결과 메시지를 보여준 뒤, `ResetGame()`을 통해 정답지를 새로 생성하고 시도 횟수를 초기화하여 다음 라운드를 준비합니다.

---

### ### 📝 핵심 코드 하이라이트

**[데이터 동기화 설정 - ynPlayerController.cpp]**
```cpp
void AynPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // NotificationText 변수를 모든 클라이언트에 복제하도록 등록
	DOREPLIFETIME(ThisClass, NotificationText);
}

void AynPlayerController::OnRep_NotificationText()
{
    // 값이 복제되었을 때 로컬 위젯에 알림(로그) 출력
	yn_nine_taskFunctionLibrary::MyprintString(this, NotificationText.ToString(), 5.f, FColor::Yellow);
}
```

---

### ### 🎯 개발 포인트
* **직관적인 UI:** `IsLocalController()` 체크를 통해 각 플레이어의 화면에 맞는 최적의 UI만 생성하도록 BeginPlay 로직을 구성했습니다.
* **유연한 데이터 구조:** `AynPlayerState`를 사용하여 접속이 끊겼다 재접속하더라도 플레이어의 점수와 상태가 유지될 수 있는 기반을 마련했습니다.

---

