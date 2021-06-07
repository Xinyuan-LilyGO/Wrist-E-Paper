/**
  ******************************************************************************
  * @file    css.h
  * @author  Waveshare Team
  * @version V1.0.0
  * @date    23-January-2018
  * @brief   This file describes the sending of Cascade Style Sheet to a client
  *
  ******************************************************************************
  */

void sendCSS(WiFiClient client)
{
client.println(
".menu_button {\r\n"
"width: 100%;\r\n"
"height: 50px;\r\n"
"border-radius: 4px;\r\n"
"text-align: center;\r\n"
"cursor: pointer;\r\n"
"display: block;\r\n"
"margin-bottom:5px;\r\n"
"font: 14px/50px Tahoma;\r\n"
"transition: all 0.18s ease-in-out;\r\n"
"border: 1px solid #4FD666;\r\n"
"background: linear-gradient(to top right, #3EC97A, #69EA49 20%, rgba(255, 255, 255, 0) 80%, rgba(255, 255, 255, 0)) top right/500% 500%;\r\n"
"color: green;\r\n"
"}\r\n"

".menu_button:hover {\r\n"
"color: white;\r\n"
"background-position: bottom left;\r\n"
"}\r\n"

".hidden_input {\r\n"
"width: 0.1px;\r\n"
"height: 0.1px;\r\n"
"opacity: 0;\r\n"
"overflow: hidden;\r\n"
"position: absolute;\r\n"
"z-index: -1;\r\n"
"}\r\n"

".imgBox\r\n"
"{\r\n"
"border: 2px dashed green;\r\n"
"border-radius: 8px;\r\n"
"background: lightyellow;\r\n"
"background-repeat: no-repeat;\r\n"
"text-align: center;\r\n"
"margin-top:10px;\r\n"
"}\r\n"

".mesBox {\r\n"
"margin: 100px 70px;\r\n"
"color: green;\r\n"
"font-size: 25px;\r\n"
"font-family: Verdana, Arial, sans-serif;\r\n"
"}\r\n"

".header_back\r\n"
"{\r\n"
"width:100%;\r\n"
"height:100px;\r\n"
"background:#EEEEEE;\r\n"
"}\r\n"

".content_back\r\n"
"{\r\n"
"width:100%;\r\n"
"background:#EEEEEE;\r\n"
"}\r\n"

".footer_back\r\n"
"{\r\n"
"width:100%;\r\n"
"height:100px;\r\n"
"background:#EEEEEE;\r\n"
"}\r\n"

".header_face\r\n"
"{\r\n"
"width:1024px;\r\n"
"height:100px;\r\n"
"margin:0 auto;\r\n"
"background:#CCFFCC;\r\n"
"}\r\n"

".content_face\r\n"
"{\r\n"
"width:1024px;\r\n"
"height:100%;\r\n"
"margin:0 auto;\r\n"
"background:#FFFFFF;\r\n"
"}\r\n"

".footer_face\r\n"
"{\r\n"
"width:1024px;\r\n"
"height:100%;\r\n"
"margin:0 auto;\r\n"
"background:#CCFFCC;\r\n"
"}\r\n"

".menu_bar\r\n"
"{\r\n"
"border-right:1px solid #4FD666;\r\n"
"background:#EEFFEE;\r\n"
"vertical-align:top;\r\n"
"padding:15px;\r\n"
"width:200px;\r\n"
"height:100%\r\n"
"}\r\n"

".content_body\r\n"
"{\r\n"
"vertical-align:top;\r\n"
"text-align:center;\r\n"
"height:100%;\r\n"
"padding:30px;\r\n"
"padding-left:15px;\r\n"
"}\r\n"

".title\r\n"
"{\r\n"
"color:green;\r\n"
"font-size:32px;\r\n"
"font-family: arial;\r\n"
"}\r\n"

".comment\r\n"
"{\r\n"
"width:100%;\r\n"
"color:green;\r\n"
"font-size:16px;\r\n"
"font-style:italic;\r\n"
"font-family: arial;\r\n"
"text-align:center;\r\n"
"}\r\n"

".nud\r\n"
"{\r\n"
"width:70px;\r\n"
"}\r\n"); 
}
