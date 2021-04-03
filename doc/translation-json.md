
원본 텍스트와 번역 정보를 담는 json 파일에 대한 문서입니다.

# 위치
`GeometryDash.exe`와 `languagePatch.dll`(이 프로젝트를 빌드하면 나오는 파일)과 같은 위치에 위치해야 합니다.

예시 - `ko-kr.json`이 번역 파일입니다:
```
Geometry Dash
  |- GeometryDash.exe
  |- languagePatch.dll
  |- ko-kr.json
```

# 포맷

예시:
```
{
  "Are you sure you want to Quit?": "정말 게임을 <cr>종료</c>하시겠습니까?",
  "<cr>Jumps</c>: %s" : {
    "format": "<cr>점프</c>: %s",
    "parts": []
  }
}
```
## `<key>: String`
번역할 원본 텍스트입니다.
크게 두 종류로 나눌 수 있습니다.

### formatString
예시: `<cr>Jumps</c>: %s`

- `sprintf`를 통해 일부가 치환되어 표시됩니다.
- `<cr>`과 같은 색 표시가 명시되어 있습니다.
- `%s`와 같은 placeholder를 적어도 하나 포함하고 있습니다.
- `Object`를 `<value>`로 받습니다.

### 그 외 문자열
예시: `Are you sure you want to Quit?`

- `formatString`을 제외한 모든 종료의 문자열입니다.
- 색 표시가 명시되어 있지 않습니다.(위 예시에서 'Quit'는 아무런 색 표시가 없지만, 실제로 게임 내에선 붉은 색으로 표시됩니다.
- `String`을 `<value>`로 받습니다.

## `<value>: String | Object`
번역된 텍스트입니다.
 
- `<key>`가 formatStr이면 `Object`를, 그 외 문자열이면 `String` 형식을 가집니다.
- `<key>`의 종료와 상관 없이 색 표시를 사용할 수 있습니다. 따라서 원본 텍스트에 색이 없는 경우에도 색을 입힐 수 있습니다.

### formatStr Object
`<key>`가 formatStr인 경우 `<value>`로 사용됩니다.
예시:
```
{
  "format": "<cr>점프</c>: %s",
  "parts": []
}
```
- format: placeholder를 포함하는 번역된 문자열입니다. `parts` 필드의 순서에 따라서 placeholder가 앞에서부터 실제 값으로 치환됩니다.
- parts: 설명 추가 예정

## 색 표시
`CCConst.h`에 정의되어 있습니다.


| 이름           | 색 표시 | RGB        |
|----------------|------|------------|
| Red            | \<cr> | 255,90,90  |
| Yellow         | \<cy> | 255,255,0  |
| Green          | \<cg> | 64,227,72  |
| Blue/Lime Blue | \<cl> | 96,171,239 |
| Purple         | \<cp> | 255,0,255  |
| Orange         | \<co> | 255,165,75 |
