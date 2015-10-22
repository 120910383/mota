#pragma once

#include "Singleton.h"

class ModalDialogManager : public Singleton<ModalDialogManager>
{
public:
    void pushDialog(cocos2d::Node* dialog);
    void popDialog();
    bool isShown();

protected:
    cocos2d::Vector<cocos2d::Layer*> _dialogs;
};

class OKCancelDialog : public cocos2d::Node
{
public:
    enum RETURN_TYPE { OK, CANCEL };
    static OKCancelDialog* create(const std::string& title, const std::string& content);

public:
    bool init(const std::string & title, const std::string & content);
    void setCallback(std::function<void(RETURN_TYPE)> callback) { _callback = callback; }

protected:
    std::function<void(RETURN_TYPE)> _callback;
};

class PromptDialog : public cocos2d::Node
{
public:
    static void show(const std::string& content);

public:
    bool init(const std::string& content);
};