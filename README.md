<img src="Resources\Screenshots\main.jpg" width=200>


魔塔大冒险开源版
=========

* [魔塔][1] 是一种起源于日本的RPG游戏，目前在中国演化出各种不同的版本。大多数魔塔以一位勇士解救被困在魔塔中的公主作为背景，玩家需要操控勇士在魔塔内行走，打败怪物，积累宝物，最终救出公主。虽然魔塔的界面很像是一般的地牢游戏，貌似随便的打打杀杀就可以过关，但事实上玩这个游戏需要动很多脑筋，任何一个轻率的选择都可能导致游戏的失败，该游戏有属性攻击、防御、生命、金币、经验。对怪物的伤害次数计算公式，是敌人的生命/（自己的攻击-敌人的防御）；而伤害的数目计算是怪物伤害次数（敌人的攻击-自己的防御）。最早的版本是两位日本人做的，引进中国后胖老鼠又制作了21层魔塔，然后就是新型作品新新魔塔，魔塔2000，后来0ksh制作了RMXP用的魔塔样板，参与制作魔塔的人也就越来越多了但制作水平不一。

* [魔塔大冒险][2] 是国内一个游戏团队做的另一款版本，我第一次接触的魔塔是高三同桌的文曲星自带的游戏，而后上了大学后又是玩了各种各样的flash魔塔游戏，该款魔塔大冒险是近期偶然在app store发现的，制作比前面几款精良许多，由于勾起了以往游戏时的快乐回忆，也想自己用cocos2d-x重写一下练练手，于是就有了这个开源项目。

##开发环境
* Windows10
* Visual Studio 2015
* [Tiled Map Editor 0.14.2][3]
* [TexturePacker 3.9.4][4]

##运行说明
1. git clone https://github.com/120910383/mota.git 到develop\mota中。
2. 解压缩官方cocos2d-x到develop\cocos2d-x，相对目录结构如下：
    -cocos2d-x
     +build
     +cmake
     +cocos
     ...
    -mota
     +Classes
     ...
     mota.sln
3. 在确保机器上已安装Visual Studio 2015后双击mota.sln即可调试运行项目。



[1]: http://baike.baidu.com/item/%E9%AD%94%E5%A1%94
[2]: http://baike.baidu.com/item/%E9%AD%94%E5%A1%94%E5%A4%A7%E5%86%92%E9%99%A9
[3]: http://www.mapeditor.org/
[4]: https://www.codeandweb.com/texturepacker
