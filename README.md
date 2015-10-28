<img src="https://github.com/120910383/mota/tree/master/Resources/Screenshots/main.jpg" width=200>
<img src="https://github.com/120910383/mota/tree/master/Resources/Screenshots/tiledmap.jpg" width=400>

魔塔大冒险开源版
=========

* [魔塔][1] 是一种起源于日本的RPG游戏，目前在中国演化出各种不同的版本。大多数魔塔以一位勇士解救被困在魔塔中的公主作为背景，玩家需要操控勇士在魔塔内行走，打败怪物，积累宝物，最终救出公主。虽然魔塔的界面很像是一般的地牢游戏，貌似随便的打打杀杀就可以过关，但事实上玩这个游戏需要动很多脑筋，任何一个轻率的选择都可能导致游戏的失败，该游戏有属性攻击、防御、生命、金币、经验。对怪物的伤害次数计算公式，是敌人的生命/（自己的攻击-敌人的防御）；而伤害的数目计算是怪物伤害次数（敌人的攻击-自己的防御）。最早的版本是两位日本人做的，引进中国后胖老鼠又制作了21层魔塔，然后就是新型作品新新魔塔，魔塔2000，后来0ksh制作了RMXP用的魔塔样板，参与制作魔塔的人也就越来越多了但制作水平不一。

* [魔塔大冒险][2] 是国内一个游戏团队做的另一款版本，该款魔塔大冒险是近期偶然在app store发现的，制作比前面几款精良许多，准备用cocos2d-x重写一下练练手，于是就有了这个开源项目。

##一些说明
* 游戏美术使用的是官方魔塔大冒险安卓apk包内的资源，如图片骨骼动画多语言，官方保留其所有权。
* 游戏代码仅限开发者交流借鉴学习，可重新编译测试，但不可发布上架。
* 可以自由使用项目中的任意代码，重新组合使用在自己项目中，但不可整个工程拿来换皮。

##开发环境
* Windows10
* Visual Studio 2015
* Android Studio 1.4 (还未添加项目文件)
* [Tiled Map Editor 0.14.2][3]
* [TexturePacker 3.9.4][4]
* OS X 10.11
* Xcode 7 (还未添加项目文件)

##运行步骤
1. git clone https://github.com/120910383/mota.git 到develop\mota中。
2. 解压缩官方cocos2d-x到develop\cocos2d-x，相对目录结构如下：    
    -cocos2d-x    
    &ensp;&ensp;+build    
    &ensp;&ensp;+cmake    
    &ensp;&ensp;+cocos    
    &ensp;&ensp;...    
    &ensp;&ensp;+setup.py    
    -mota    
    &ensp;&ensp;+Classes    
    &ensp;&ensp;...    
    &ensp;&ensp;mota.sln    
3. 在确保机器上已安装Visual Studio 2015后双击mota.sln即可调试运行项目。

##参与项目
1. 可以加入QQ群490412185（cocos2d-x开源项目交流群）讨论相关技术或者共同贡献代码<a target="_blank" href="http://shang.qq.com/wpa/qunwpa?idkey=32be4fcda35f3cbdab4ae005b235714c17abfdcc1c456da42911298d11db4630"><img border="0" src="http://pub.idqqimg.com/wpa/images/group.png" alt="cocos2d-x开源项目交流" title="cocos2d-x开源项目交流"></a>
2. 在github上直接提交Issue，pull request
3. 可以关注我自己的开发过程[issue][5]
4. 目前需要程序若干（和我一起写代码，一个人太累而且没动力），数值策划若干（其实就是整理原游戏中怪物数值信息，以后看能不能自己来设计剧情）


[1]: http://baike.baidu.com/item/%E9%AD%94%E5%A1%94
[2]: http://baike.baidu.com/item/%E9%AD%94%E5%A1%94%E5%A4%A7%E5%86%92%E9%99%A9
[3]: http://www.mapeditor.org/
[4]: https://www.codeandweb.com/texturepacker
[5]: http://develop.hooyio.com/projects/mota/issues