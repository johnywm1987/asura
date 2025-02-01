#pragma once

#include <boost/signals2.hpp>

    /*! ��ũ�� ����
    Text        : �˾� �ؽ�Ʈ (Description)
    ClassName   : Ŭ���� �̸� (ClassName)
    Function    : �Լ��̸� (Function)
    Formula     : ��
    */
#define CREATEPOPUPITEM( Text, ClassName, Function ) \
    popupmenu::CreatePopupMenuItem( Text, boost::bind(&ClassName::Function, this, _1) )
#define CREATEPOPUPITEM2( Text, ClassName, Function, Formula ) \
    ( Formula )?popupmenu::CreatePopupMenuItem( Text, boost::bind(&ClassName::Function, this, _1) ):popupmenu::CreatePopupMenuItem( Text )
    

namespace popupmenu
{
    class PopupMenuComponent;
    class PopupMenuComposite;
    class PopupMenuItem;

    typedef std::tr1::shared_ptr<PopupMenuComponent> SP_POPUP_MENU;
    typedef boost::function< void(int) > POPUP_FUNCTION;

    popupmenu::SP_POPUP_MENU CreateRoot();
    popupmenu::SP_POPUP_MENU CreatePopupMenuComposite( const std::string& _Description, bool _Enable );
    popupmenu::SP_POPUP_MENU CreatePopupMenuItem( const std::string& _Description, POPUP_FUNCTION CallbackFunc = NULL );

    struct POPUP_NODE
    {
        std::string Description;
        bool Enable;

        POPUP_NODE( const std::string& _Description, bool _Enable )
            : Description(_Description)
            , Enable(_Enable)
        {
        }
    };
}
