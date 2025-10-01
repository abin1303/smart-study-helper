# Smart Study Helper

Smart Study Helper는 임베디드 디바이스, 라즈베리 파이 서버, 웹 대시보드를 연결해 열람실 좌석과 환경 정보를 통합 관리하는 IoT 실습 프로젝트입니다.

## 저장소 구성

- `arduino/` - Wi-Fi 기능의 아두이노 스톱워치. 초음파/조도 센서와 I2C LCD를 사용해 좌석 사용 시간을 측정하고 서버와 데이터를 주고받습니다.
- `mini_project/` - STM32F4 기반 STM32CubeIDE 펌웨어. 키패드, RGB LED, CLCD, DHT 센서, Bluetooth/UART 통신을 처리합니다.
- `raspberrypi_server_client/` - 라즈베리 파이용 C 프로그램. 멀티스레드 TCP 서버와 Bluetooth/도서관 클라이언트 예제가 포함됩니다.
- `db_php_html/` - MySQL에 저장된 좌석/환경 데이터를 보여주는 PHP 스크립트와 HTML 프레임셋.
- `README.md` - 프로젝트 문서.

## 시스템 개요

1. 아두이노와 STM32 장치는 좌석 점유 정보와 온습도 등 환경 데이터를 수집합니다.
2. 각 장치는 Wi-Fi 또는 Bluetooth를 통해 라즈베리 파이 서버(`iot_server.c`)에 연결하고 `idpasswd.txt`에 정의된 ID/비밀번호로 인증합니다.
3. 서버는 디바이스 간 메시지를 중계하고 접속 로그를 표준 출력으로 남깁니다. 필요 시 클라이언트가 현재 시각 조회나 브로드캐스트 명령을 보낼 수 있습니다.
4. PHP 페이지는 `Library_Member` 데이터베이스를 조회해 DHT 센서 기록과 좌석 배정을 웹 대시보드로 제공합니다.

## 시작하기

### 아두이노 스톱워치 (`arduino/`)
- 준비물: Arduino Uno(또는 호환 보드), ESP8266(WiFiEsp), 초음파 센서(TRIG/ECHO), CDS 조도 센서, 16x2 I2C LCD, 버튼.
- 필요한 라이브러리: `WiFiEsp`, `LiquidCrystal_I2C`, `TimerOne`, `SoftwareSerial`.
- `mini_project_stopwatch_finish.ino`에서 `SERVER_NAME`, `AP_SSID`, `AP_PASS`, 좌석 번호를 환경에 맞게 수정합니다.
- Arduino IDE로 업로드하면 시간 측정 결과를 LCD와 시리얼로 출력하고 서버 명령(예: LED 제어, GETTIME)에 응답합니다.

### STM32 CubeIDE 프로젝트 (`mini_project/`)
- STM32CubeIDE에서 폴더를 열고(테스트 보드: STM32F411RE Nucleo) 빌드하면 산출물은 `Debug/`에 생성됩니다.
- 펌웨어는 TIM3 PWM으로 RGB LED를 제어하고, CLCD에 메시지를 표시하며, DHT 센서를 읽어 UART(Bluetooth)로 전송합니다.
- Wi-Fi/Bluetooth 로직과 LCD 문구는 `Core/Src/main.c`에서 수정할 수 있습니다.

### 라즈베리 파이 서버 및 클라이언트 (`raspberrypi_server_client/`)
- 요구 사항: GCC, POSIX 스레드(`-lpthread`).
- 컴파일 예시:
  ```sh
  gcc -o iot_server iot_server.c -lpthread
  gcc -o iot_client_bluetooth iot_client_bluetooth.c
  gcc -o iot_client_library iot_client_library.c
  ```
- `idpasswd.txt`에 각 디바이스의 `ID PASSWORD` 쌍을 한 줄씩 추가합니다.
- `./iot_server <port>` 형태로 실행하면 디바이스는 `[ID:PASS]` 헤더로 인증하고, 서버는 콘솔에 연결/메시지 로그를 출력합니다.

### 웹 대시보드 (`db_php_html/`)
- Apache/PHP 서버에 파일을 배포하고 MySQL 접근 권한을 설정합니다.
- `Library_Member` 데이터베이스와 `DHT_data` 테이블을 PHP 쿼리에 맞게 생성합니다.
- 서버 IP가 기본값 `10.10.141.85`와 다르면 PHP/HTML 파일의 URL을 수정합니다.
- `index.html`을 열면 좌석, 환경, 회원 정보를 프레임셋으로 확인할 수 있습니다.

## 개발 참고사항

- 네트워크 SSID, 비밀번호, 서버 IP는 실습 환경 기준으로 하드코딩되어 있으므로 배포 전 재설정이 필요합니다.
- 빌드 산출물과 로그 파일은 `.gitignore`에 등록되어 버전 관리에서 제외됩니다.
- 소스 파일에는 한글 주석이 포함되어 있으니 UTF-8 인코딩을 유지한 채 편집하세요.
- 디바이스를 추가하려면 `idpasswd.txt`에 계정을 등록하고 서버 로직이 메시지를 처리할 수 있는지 확인하세요.

## 향후 작업

- 서버에서 MySQL 업데이트를 자동화해 클라이언트 없이도 데이터가 저장되도록 개선.
- 서버 메시지 라우팅에 대한 단위/통합 테스트 추가.
- PHP/MySQL 환경을 컨테이너로 구성해 로컬 개발 환경 구축을 단순화.
