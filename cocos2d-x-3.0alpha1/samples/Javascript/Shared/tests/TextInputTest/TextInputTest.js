/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2011      Zynga Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

var TEXT_INPUT_FONT_NAME = "Thonburi";
var TEXT_INPUT_FONT_SIZE = 36;

var sceneIdx = -1;

var textInputGetRect = function (node) {
    var pos = node.getPosition();
    var cs = node.getContentSize();
    var rc = cc.rect(pos.x, pos.y, cs.width, cs.height);
    rc.x -= rc.width / 2;
    rc.y -= rc.height / 2;
    return rc;
};

/**
 @brief    TextInputTest for retain prev, reset, next, main menu buttons.
 */
var TextInputTest = cc.Layer.extend({
    notificationLayer:null,
    ctor:function() {
        this._super();
        this.init();
    },

    restartCallback:function (sender) {
        var scene = new TextInputTestScene();
        scene.addChild(restartTextInputTest());
        cc.Director.getInstance().replaceScene(scene);
    },
    nextCallback:function (sender) {
        var scene = new TextInputTestScene();
        scene.addChild(nextTextInputTest());
        cc.Director.getInstance().replaceScene(scene);
    },
    backCallback:function (sender) {
        var scene = new TextInputTestScene();
        scene.addChild(previousTextInputTest());
        cc.Director.getInstance().replaceScene(scene);
    },

    title:function () {
        return "text input test";
    },

    addKeyboardNotificationLayer:function (layer) {
        this.notificationLayer = layer;
        this.addChild(layer);
    },

    onEnter:function () {
        this._super();

        var winSize = cc.Director.getInstance().getWinSize();

        var label = cc.LabelTTF.create(this.title(), "Arial", 24);
        this.addChild(label);
        label.setPosition(cc.p(winSize.width / 2, winSize.height - 50));

        var subTitle = this.subtitle();
        if (subTitle && subTitle !== "") {
            var l = cc.LabelTTF.create(subTitle, "Thonburi", 16);
            this.addChild(l, 1);
            l.setPosition(cc.p(winSize.width / 2, winSize.height - 80));
        }

        var item1 = cc.MenuItemImage.create(s_pathB1, s_pathB2, this.backCallback, this);
        var item2 = cc.MenuItemImage.create(s_pathR1, s_pathR2, this.restartCallback, this);
        var item3 = cc.MenuItemImage.create(s_pathF1, s_pathF2, this.nextCallback, this);

        var menu = cc.Menu.create(item1, item2, item3);
        menu.setPosition(cc.p(0,0));
        item1.setPosition(cc.p(winSize.width / 2 - 100, 30));
        item2.setPosition(cc.p(winSize.width / 2, 30));
        item3.setPosition(cc.p(winSize.width / 2 + 100, 30));

        this.addChild(menu, 1);
    }
});

//////////////////////////////////////////////////////////////////////////
// KeyboardNotificationLayer for test IME keyboard notification.
//////////////////////////////////////////////////////////////////////////
var KeyboardNotificationLayer = TextInputTest.extend({
    _trackNode:null,
    _beginPos:null,

    ctor:function () {
        this._super();

        if( 'touches' in sys.capabilities )
            this.setTouchEnabled(true);
        else if ('mouse' in sys.capabilities )
            this.setMouseEnabled(true);
    },

    subtitle:function () {
        return "";
    },

    onClickTrackNode:function (clicked) {
    },

    keyboardWillShow:function (info) {
        cc.log("TextInputTest:keyboardWillShowAt(origin:" + info.end.x + "," + info.end.y
            + ", size:" + info.end.width + "," + info.end.height + ")");

        if (!this._trackNode)
            return;

        var rectTracked = textInputGetRect(this._trackNode);
        cc.log("TextInputTest:trackingNodeAt(origin:" + info.end.x + "," + info.end.y
            + ", size:" + info.end.width + "," + info.end.height + ")");

        // if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
        if (!cc.rectIntersectsRect(rectTracked, info.end))
            return;

        // assume keyboard at the bottom of screen, calculate the vertical adjustment.
        var adjustVert = cc.rectGetMaxY(info.end) - cc.rectGetMinY(rectTracked);
        cc.log("TextInputTest:needAdjustVerticalPosition(" + adjustVert + ")");

        // move all the children node of KeyboardNotificationLayer
        var children = this.getChildren();
        for (var i = 0; i < children.length; ++i) {
            var node = children[i];
            var pos = node.getPosition();
            pos.y += adjustVert;
            node.setPosition(pos);
        }
    },

    onTouchesEnded:function (touches, event) {
        if (!this._trackNode)
            return;

        // grab first touch
        if(touches.length == 0)
            return;

        var touch = touches[0];
        var point = touch.getLocation();

        // decide the trackNode is clicked.
        cc.log("KeyboardNotificationLayer:clickedAt(" + point.x + "," + point.y + ")");

        var rect = textInputGetRect(this._trackNode);
        cc.log("KeyboardNotificationLayer:TrackNode at(origin:" + rect.x + "," + rect.y
            + ", size:" + rect.width + "," + rect.height + ")");

        this.onClickTrackNode(cc.rectContainsPoint(rect, point));
        cc.log("----------------------------------");
    },
    onMouseUp:function (event) {
        if (!this._trackNode)
            return;

        var point = event.getLocation();

        // decide the trackNode is clicked.
        cc.log("KeyboardNotificationLayer:clickedAt(" + point.x + "," + point.y + ")");

        var rect = textInputGetRect(this._trackNode);
        cc.log("KeyboardNotificationLayer:TrackNode at(origin:" + rect.x + "," + rect.y
            + ", size:" + rect.width + "," + rect.height + ")");

        this.onClickTrackNode(cc.rectContainsPoint(rect, point));
        cc.log("----------------------------------");
    }
});

//////////////////////////////////////////////////////////////////////////
// TextFieldTTFDefaultTest for test TextFieldTTF default behavior.
//////////////////////////////////////////////////////////////////////////
var TextFieldTTFDefaultTest = KeyboardNotificationLayer.extend({
    subtitle:function () {
        return "TextFieldTTF with default behavior test";
    },
    onClickTrackNode:function (clicked) {
        var textField = this._trackNode;
        if (clicked) {
            // TextFieldTTFTest be clicked
            cc.log("TextFieldTTFDefaultTest:CCTextFieldTTF attachWithIME");
            textField.attachWithIME();
        } else {
            // TextFieldTTFTest not be clicked
            cc.log("TextFieldTTFDefaultTest:CCTextFieldTTF detachWithIME");
            textField.detachWithIME();
        }
    },

    onEnter:function () {
        this._super();

        // add CCTextFieldTTF
        var winSize = cc.Director.getInstance().getWinSize();

        var textField = cc.TextFieldTTF.create("<click here for input>",
            TEXT_INPUT_FONT_NAME,
            TEXT_INPUT_FONT_SIZE);
        this.addChild(textField);
        textField.setPosition(cc.p(winSize.width / 2, winSize.height / 2));

        this._trackNode = textField;
    }
});

//////////////////////////////////////////////////////////////////////////
// TextFieldTTFActionTest
//////////////////////////////////////////////////////////////////////////
var TextFieldTTFActionTest = KeyboardNotificationLayer.extend({
    _textField:null,
    _textFieldAction:null,
    _action:false,
    _charLimit:0, // the textfield max char limit

    ctor:function () {
        this._super();
    },

    callbackRemoveNodeWhenDidAction:function (node) {
        this.removeChild(node, true);
    },

    // KeyboardNotificationLayer
    subtitle:function () {
        return "CCTextFieldTTF with action and char limit test";
    },
    onClickTrackNode:function (clicked) {
        var textField = this._trackNode;
        if (clicked) {
            // TextFieldTTFTest be clicked
            cc.log("TextFieldTTFActionTest:CCTextFieldTTF attachWithIME");
            textField.attachWithIME();
        } else {
            // TextFieldTTFTest not be clicked
            cc.log("TextFieldTTFActionTest:CCTextFieldTTF detachWithIME");
            textField.detachWithIME();
        }
    },

    //CCLayer
    onEnter:function () {
        this._super();

        this._charLimit = 20;
        this._textFieldAction = cc.RepeatForever.create(
            cc.Sequence.create(
                cc.FadeOut.create(0.25),
                cc.FadeIn.create(0.25)));
        this._action = false;

        // add CCTextFieldTTF
        var winSize = cc.Director.getInstance().getWinSize();

        this._textField = cc.TextFieldTTF.create("<click here for input>",
            TEXT_INPUT_FONT_NAME,
            TEXT_INPUT_FONT_SIZE);
        this.addChild(this._textField);
        this._textField.setDelegate(this);

        this._textField.setPosition(cc.p(winSize.width / 2, winSize.height / 2));
        this._trackNode = this._textField;
    },

    //CCTextFieldDelegate
    onTextFieldAttachWithIME:function (sender) {
        if (!this._action) {
            this._textField.runAction(this._textFieldAction);
            this._action = true;
        }
        return false;
    },
    onTextFieldDetachWithIME:function (sender) {
        if (this._action) {
            this._textField.stopAction(this._textFieldAction);
            this._textField.setOpacity(255);
            this._action = false;
        }
        return false;
    },
    onTextFieldInsertText:function (sender, text, len) {
        // if insert enter, treat as default to detach with ime
        if ('\n' == text) {
            return false;
        }

        // if the textfield's char count more than m_nCharLimit, doesn't insert text anymore.
        if (sender.getCharCount() >= this._charLimit) {
            return true;
        }

        // create a insert text sprite and do some action
        var label = cc.LabelTTF.create(text, TEXT_INPUT_FONT_NAME, TEXT_INPUT_FONT_SIZE);
        this.addChild(label);
        var color = new cc.Color3B(226, 121, 7);
        label.setColor(color);

        // move the sprite from top to position
        var endPos = cc.p(sender.getPositionX(), sender.getPositionY());
        if (sender.getCharCount()) {
            endPos.x += sender.getContentSize().width / 2;
        }
        var inputTextSize = label.getContentSize();
        var beginPos = cc.p(endPos.x, cc.Director.getInstance().getWinSize().height - inputTextSize.height * 2);

        var duration = 0.5;
        label.setPosition(beginPos);
        label.setScale(8);

        var seq = cc.Sequence.create(
            cc.Spawn.create(
                cc.MoveTo.create(duration, endPos),
                cc.ScaleTo.create(duration, 1),
                cc.FadeOut.create(duration)),
            cc.CallFunc.create(this.callbackRemoveNodeWhenDidAction, this));
        label.runAction(seq);
        return false;
    },

    onTextFieldDeleteBackward:function (sender, delText, len) {
        // create a delete text sprite and do some action
        var label = cc.LabelTTF.create(delText, TEXT_INPUT_FONT_NAME, TEXT_INPUT_FONT_SIZE);
        this.addChild(label);

        // move the sprite to fly out
        var beginPos = cc.p(sender.getPositionX(), sender.getPositionY());
        var textfieldSize = sender.getContentSize();
        var labelSize = label.getContentSize();
        beginPos.x += (textfieldSize.width - labelSize.width) / 2.0;

        var winSize = cc.Director.getInstance().getWinSize();
        var endPos = cc.p(-winSize.width / 4.0, winSize.height * (0.5 + Math.random() / 2.0));

        var duration = 1;
        var rotateDuration = 0.2;
        var repeatTime = 5;
        label.setPosition(beginPos);

        var seq = cc.Sequence.create(
            cc.Spawn.create(
                cc.MoveTo.create(duration, endPos),
                cc.Repeat.create(
                    cc.RotateBy.create(rotateDuration, (Math.random() % 2) ? 360 : -360),
                    repeatTime),
                cc.FadeOut.create(duration)),
            cc.CallFunc.create(this.callbackRemoveNodeWhenDidAction, this));
        label.runAction(seq);
        return false;
    },
    onDraw:function (sender) {
        return false;
    }
});

var TextInputTestScene = TestScene.extend({
    runThisTest:function () {
        sceneIdx = -1;
        var layer = nextTextInputTest();

        this.addChild(layer);
        cc.Director.getInstance().replaceScene(this);
    }
});

//
// Flow control
//
var arrayOfTextInputTest = [
    TextFieldTTFDefaultTest,
    TextFieldTTFActionTest
];

var nextTextInputTest = function () {
    sceneIdx++;
    sceneIdx = sceneIdx % arrayOfTextInputTest.length;

    return new arrayOfTextInputTest[sceneIdx]();
};
var previousTextInputTest = function () {
    sceneIdx--;
    if (sceneIdx < 0)
        sceneIdx += arrayOfTextInputTest.length;

    return new arrayOfTextInputTest[sceneIdx]();
};
var restartTextInputTest = function () {
    return new arrayOfTextInputTest[sceneIdx]();
};

