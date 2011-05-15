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

// Swipe detection borrowed from:
// http://developer.qt.nokia.com/forums/viewthread/3290


import Qt 4.7


MouseArea {
   signal swipeRight;
   signal swipeLeft;
   signal longPress;

   anchors.fill: parent
   hoverEnabled: true

   property int startX;
   property int startY;

   onPressed: {
       startX = mouse.x;
       startY = mouse.y;
   }

   onReleased: {
       var deltax = mouse.x - startX;
       var deltay = mouse.y - startY;

       if (Math.abs(deltax) > 50 || Math.abs(deltay) > 50) {
          if (deltax > 30 && Math.abs(deltay) < 30) {
             swipeRight()
             console.log("Swipe right")
          } else if (deltax < -30 && Math.abs(deltay) < 30) {
             swipeLeft()
             console.log("Swipe left")
          }
       }
   }

   onPressAndHold: {
       longPress();
   }

}
