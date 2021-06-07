/**
  ******************************************************************************
  * @file    html.h
  * @author  Waveshare Team
  * @version V1.0.0
  * @date    23-January-2018
  * @brief   This file describes the sending of HTML-page to a client
  *
  ******************************************************************************
  */

void sendHtml(WiFiClient client,IPAddress myIP)
{
client.print(
"<!DOCTYPE html>\r\n"
"<html>\r\n"
"<head>\r\n"
"<meta charset='utf-8'>\r\n"
"<title>PriceTag</title>\r\n"
"<link rel='icon' href='data:;base64,='>\r\n"
"<link rel='stylesheet' href='styles.css'>\r\n"
"<script src='scriptA.js'></script>\r\n"
"<script src='scriptB.js'></script>\r\n"
"<script src='scriptC.js'></script>\r\n"
"<script src='scriptD.js'></script>\r\n"
"</head>\r\n"
"<body>\r\n"
"<div class='header_back'>\r\n"
"<div class='header_face'></div>\r\n"
"</div>\r\n"
"<div class='content_back'>\r\n"
"<div class='content_face'>\r\n"
"<table style='width:100%; height:100%' border='0' cellspacing='0'>\r\n"
"<tr>\r\n"
"<td class='menu_bar' rowspan='2'>\r\n"
"<div id='BT'></div>\r\n"
"<table>\r\n"
"<tr>\r\n"
"<td class='comment' colspan='4'><br>Device IP:</td>\r\n"
"</tr>\r\n"               
"<tr>\r\n"
"<td colspan=\"4\"><input id=\"ip_addr\" style=\"width:100%;text-align:center\" type=\"text\" value=\"\r\n"
+ String(myIP[0],DEC)+'.'+String(myIP[1],DEC)+'.'+String(myIP[2],DEC)+'.'+String(myIP[3],DEC)+"\"></td>\r\n"
"</tr>\r\n"               
"<tr><td class='comment' colspan='4'><br>Bounds:</td></tr>\r\n" 
"<tr id='XY'></tr>\r\n"
"<tr id='WH'></tr>\r\n"
"</table>\r\n"       
"<div id='RB'></div>\r\n"
"</td>\r\n"
"<td class='content_body'>\r\n"
"<span class='title'>Original image</span><br>\r\n"
"<canvas id='source' class='hidden_input'></canvas>\r\n"        
"<div id='srcBox'>\r\n"
"<div class='imgBox'>\r\n"
"<div class='mesBox'>Drop image here...</div>\r\n"
"</div>\r\n"
"</div>\r\n"            
"</td>\r\n"      
"</tr>\r\n"
"<tr>\r\n"
"<td id='dstBox' class='content_body'>\r\n" 
"</td>\r\n"        
"</tr>\r\n"
"</table>\r\n"
"</div>\r\n"
"</div>\r\n"
"<div class='footer_back'>\r\n"
"<div class='footer_face' id='logTag'>\r\n"
"</div>\r\n"
"</div>\r\n"
"</body>\r\n"
"</html>\r\n");
}
