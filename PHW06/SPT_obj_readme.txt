snnnnnnH06.cpp를 빌드할 때 환경에 따라 첨부한 object 코드중 하나를
 visual studio에 snnnnnnH06.cpp와 함께 등록해서 빌드해야 합니다.
솔루션 탐색기의 "리소스 파일"에 ~.obj 파일을 등록하면 됩니다(다른 곳에
 등록해도 가능할 것입니다. 확인 필요).

아래 object 코드는 snnnnnnH06.cpp에서 Tree_Check(), SPT_test() 함수를
 포함하고 있습니다.

SPT_test_D.obj   : 디버드 모드 compiled object file (x86, VS17 or VS19)
SPT_test_R17.obj : 릴리즈 모드 compiled object file (x86, VS17 only)
SPT_test_R19.obj : 릴리즈 모드 compiled object file (x86, VS19 only)