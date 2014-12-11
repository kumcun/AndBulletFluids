/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.bullet;

// Wrapper for native library

public class DemoLib {

     static {
         System.loadLibrary("demobullet");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void stepLeft(int width, int height);
     public static native void stepFront(int width, int height);
     public static native void stepBack(int width, int height);
     public static native void stepRight(int width, int height);
     public static native void planeRight();
     public static native void planeLeft();
     public static native void change(int width, int height);
     public static native void step();
     public static native void create();
     public static native void add(float normalizedX, float normalizedY, int heightPixels, int widthPixels);
}
