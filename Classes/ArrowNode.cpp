#include "pre_header.h"

#include "ArrowNode.h"

USING_NS_CC;

bool ArrowNode::init()
{
    auto a5 = Sprite::create("Images/a5.png");
    if (nullptr != a5)
    {
        a5->setPosition(Vec2::ZERO);
        this->addChild(a5);
    }
    auto a11 = Sprite::create("Images/a11.png");
    if (nullptr != a11)
    {
        a11->setPosition(Vec2::ZERO);
        a11->setScale(0.6f);
        auto fade = Sequence::createWithTwoActions(FadeOut::create(1.0f), FadeIn::create(0.0f));
        auto scale = Sequence::createWithTwoActions(ScaleTo::create(1.0f, 2.0f), ScaleTo::create(0.0f, 0.6f));
        a11->runAction(RepeatForever::create(Spawn::createWithTwoActions(fade, scale)));
        this->addChild(a11);
    }
    auto arrow = Sprite::create("Images/goal_jian.png");
    if (nullptr != arrow)
    {
        arrow->setScale(0.5f);
        arrow->setAnchorPoint(Vec2(0.5f, 0.0f));
        arrow->setPosition(Vec2::ZERO);
        arrow->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveBy::create(0.5f, Vec2(0, 10.0f)), MoveBy::create(0.5f, Vec2(0, -10.0f)))));
        this->addChild(arrow);
    }

    return true;
}
