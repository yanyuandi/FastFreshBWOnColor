## 7.5寸三色墨水屏黑白局部快刷驱动
![image](jpg/109A0671.jpg)

### 项目简介
此项目在三色墨水屏上增加了黑白两色的局部快刷，并且集成到<a target="_blank" href="https://github.com/ZinggJM/GxEPD2">GxEPD2</a>库中，原库中的函数可以正常使用，使用黑白局部快刷的时候调用新加函数即可 <br>
- **B站视频演示地址：[点击观看视频](https://www.bilibili.com/video/BV1Fo4y137n9/?vd_source=a935ffb91a0eac4e16a0461b0709faa9)**<br>
- <a target="_blank" href="https://qm.qq.com/cgi-bin/qm/qr?k=OCk2mwPC4yZn-BBJlH2ehWT-2sHfC7Os&jump_from=webapi&authKey=iFtohDmv6OI7O5aD/0ogd6mODvY5vr837fherj6ruuDCK94UM5KrjicZ2cFO5dHB"><img border="0" src="http://pub.idqqimg.com/wpa/images/group.png" alt="墨水屏DIY交流群" title="墨水屏DIY交流群"></a>QQ交流群：1051455459


### 使用的硬件
- 一个使用<a target="_blank" href="https://github.com/ZinggJM/GxEPD2">GxEPD2</a>库中GxEPD2_750c_Z08驱动的7.5寸三色墨水屏，驱动芯片为UC8179
- 一个ESP32C3墨水屏开发板（闲鱼搜用户<光芒之轻>有成品售卖）

### 7.5inch_Multifunctional_E-Paper文件夹
- 此文件夹中包含驱动墨水屏主要代码，包含了获取天气日期微博热搜等数据代码以及显示代码，需要使用arduino编译，编译之前注意头文件引用的库，没有的需要单独安装。<br>

- 代码需要修改每个get***.ino文件中的api地址，例如  


``` python
http.begin(Client, "http://日期.php");
``` 



### 墨水屏显示整个逻辑介绍
- 第一次上电或者更换无线网络环境后，开机后会首先显示开机画面，然后无线网连接失败会进入配网界面  
- 配网成功后等待片刻，会进入整个界面，所有数据分别获取然后统一刷新
- 整个墨水屏会两个小时全刷一次，每次刷新都卡在整点（由于没有局刷，所以目前是整个刷新）  
- 待办事件10秒钟会获取一次服务器的数据进行对比，如果有更新，会马上刷新墨水屏  
- 一言api每次刷新都会改变内容  
- 下方向按键按下后可以立马刷新墨水屏所有内容  

### 已知bug
- 第一次上电后右上角的刷新于后面的时间会更新错误，手动刷新一次即可（待完善）  


### 制作不易，感谢支持
![image](jpg/109A0671.jpg)
