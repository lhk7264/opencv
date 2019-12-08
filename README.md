# CarNumBer_Recognition


### 관련 뉴스
[신규 자동차 번호판 도입 및 차량번호 시스템 업데이트 안내!!](http://www.gangnam.go.kr/board/article/1971/view.do?mid=FM0501&schArticle=ARTICLE_01)

[카메라 인식 안돼 주차료 정산혼란 우려](https://www.yna.co.kr/view/AKR20190826065400003)

["'8자리' 번호판은 처음이죠?"](https://post.naver.com/viewer/postView.nhn?volumeNo=23081769&memberNo=38212397)

[8자리 자동차 번호판 체계 도입…민간 차량 인식기 갱신 미비](https://www.cbnews.co.kr/news/articleView.html?idxno=10498)


##### Project Name : Raspberry Pi 차량 출입 관리 시스템

##### Server>>Ubuntu 16.04

##### Client>>Raspberry pi 3 model rev 1.2(Raspbian Buster),opencv 4.1.2,Raspberry pi Camera v2 Module

##### 1 기능 : 8자리 차량 번호판 추출

##### 2 기능 : 차량 번호판 추출 차량 번호 인식 

##### 3 기능 : 인식 된 차량 번호 텍스트화


#### 1-original
![initial](https://user-images.githubusercontent.com/49156302/70368402-414b6f80-18ed-11ea-9b04-4e8cd1440075.JPG)
#### 2-change gray
![initial](https://user-images.githubusercontent.com/49156302/70368403-41e40600-18ed-11ea-9e1a-5b31defd7e8a.JPG)
#### 3-Edge
![initial](https://user-images.githubusercontent.com/49156302/70368404-41e40600-18ed-11ea-9234-78d139fdda9a.JPG)
#### 4-FindContours
![initial](https://user-images.githubusercontent.com/49156302/70368405-41e40600-18ed-11ea-9077-15e1c5bdf6fa.JPG)
#### 5-Filtering_contour
![initial](https://user-images.githubusercontent.com/49156302/70368406-41e40600-18ed-11ea-8352-7ff1123c46c0.JPG)
#### 6-Filtering_contour2
![initial](https://user-images.githubusercontent.com/49156302/70368407-427c9c80-18ed-11ea-8e5e-cbaae6f432b4.JPG)
#### 7-Merge_1 Group
![initial](https://user-images.githubusercontent.com/49156302/70368408-427c9c80-18ed-11ea-8dc0-3c9ce972ee54.JPG)
#### 8-Merge_2 Group
![initial](https://user-images.githubusercontent.com/49156302/70368409-427c9c80-18ed-11ea-942c-b1e080584fee.JPG)
