# unix
유닉스 프로그래밍 수업을 들으며 작성한 코드들을 정리한 repo입니다.<br/>
시스템콜을 이용하여 간단한 명령들을 만들어보았습니다.<br/>

## minishell 실행 방법
```yml
1. git clone 명령을 이용하여 서버에 clone합니다.
2. home 디렉토리에 bin 디렉토리를 생성합니다.
3. gcc 컴파일러를 이용하여 작성된 파일들을 컴파일한 뒤, 실행 파일을 bin 디렉토리로 옮깁니다.
4. minishell.c 파일을 컴파일한 뒤 실행합니다.
5. exit을 입력하면 종료합니다.
```
### 명령 설명
각 명령들은 다음과 같이 동작합니다.
```yml
cat_m FILE_NAME: 해당 파일의 내용을 화면에 출력합니다.
cd_m DIRECTORY_NAME: 해당 이름의 directory로 이동합니다.
cp_m FILE_NAME1 FILE_NAME2: FILE_NAME1의 내용을 FILE_NAME2에 복사합니다.
mkdir_m DIRECTORY_NAME: 해당 이름의 subdirectory를 만듭니다.
ls_m: current working directory 내에 있는 모든 파일과 subdirectory의 이름을 출력합니다.
rm_m FILE_NAME or DIRECTORY_NAME: FILE_NAME인 경우 해당 파일을 삭제합니다. DIRECTORY_NAME인 경우 해당 디렉토리 내의 파일들을 삭제하고, 해당 디렉토리를 삭제합니다. 해당 디렉토리가 subdirectory를 포함하는 경우는 없다고 가정합니다.
rmdir_m DIRECTORY_NAME: DIRECTORY_NAME인 디렉토리를 삭제합니다. 디렉토리가 비어있지 않으면 실패합니다.
vi_m FILE_NAME: FILE_NAME인 파일이 있는 경우 파일에 있는 내용을 보여줍니다. 내용을 작성하여 파일에 저장할 수 있으며, quit을 입력하면 작성을 종료합니다.
```
