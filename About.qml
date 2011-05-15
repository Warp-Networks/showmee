/*
*
 *  ShowMee - Presentation software with touch interface
 *
 *  Copyright (C) 2011   Warp Networks, S.L. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


import Qt 4.7

Rectangle {
    signal dismiss;


    width: 620 ; height: 200
    color: "black"
    radius: 10
    smooth: true
    z: 0
    opacity: 0

   Text {
      text: "<p><b>ShowMee</b>
<p>A gesture-based presentation software
<br/>
<p>Copyright 2011 <a href=\"http://warp.es\">Warp Networks, S.L.</a>
<p>Authors:
<ul>
<li>Héctor Blanco Alcaine
<li>Javier Uruen Val
</ul>"
      color: "white"
      z: 1
   }

   Image {
      anchors.right: parent.right
      source: "images/warp_big.png"
      z: 0
   }

   MouseArea {
       anchors.fill: parent
       onClicked: {
           dismiss()
       }
   }

}
