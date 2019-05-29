package com.maq.xprize.kitkitlauncher.hindi;


import android.content.Context;
import android.graphics.Point;
import android.os.RemoteException;
import android.provider.ContactsContract;
import android.support.test.InstrumentationRegistry;
//import android.support.test.espresso.intent.Intents;
import android.support.test.espresso.action.ViewActions;
import android.support.test.filters.LargeTest;
import android.support.test.rule.ActivityTestRule;
import android.support.test.rule.GrantPermissionRule;
import android.support.test.runner.AndroidJUnit4;
import android.support.test.uiautomator.UiDevice;
import android.support.test.uiautomator.UiObject;
import android.support.test.uiautomator.UiObjectNotFoundException;
import android.support.test.uiautomator.UiSelector;
import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;
import static android.support.test.InstrumentationRegistry.getInstrumentation;
import static android.support.test.espresso.Espresso.onView;
import static android.support.test.espresso.action.ViewActions.click;
import static android.support.test.espresso.assertion.ViewAssertions.matches;
import static android.support.test.espresso.matcher.ViewMatchers.isDisplayed;
import static android.support.test.espresso.matcher.ViewMatchers.isRoot;
import static android.support.test.espresso.matcher.ViewMatchers.withId;
import static org.junit.Assert.assertEquals;


import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Build;
import android.support.test.uiautomator.UiDevice;
import android.support.test.uiautomator.UiObject;
import android.support.test.uiautomator.UiObjectNotFoundException;
import android.support.test.uiautomator.UiSelector;
import android.support.v4.content.ContextCompat;
import android.util.Log;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.concurrent.TimeUnit;

import static android.support.test.InstrumentationRegistry.getInstrumentation;

@LargeTest
@RunWith(AndroidJUnit4.class)
public class Test_Deny {
    private static final int MAIN_APP_BUTTON_INSTANCE= 1;
    private static final int LIBRARY_BUTTON_INSTANCE = 2;
    private static final int TOOL_BUTTON_INSTANCE= 3;
    private static final int BACK_ARROW_INSTANCE= 0;
    private static final int TOOL_1_INSTANCE= 1;
    private static final int TOOL_2_INSTANCE= 2;
    private static final int TOOL_3_INSTANCE= 3;
    private static final int TOOL_4_INSTANCE= 4;
    private static final int TOOL_5_INSTANCE= 5;
    private static final int TOOL_6_INSTANCE= 6;
    @Rule
    public ActivityTestRule<MainActivity> mActivityRule = new ActivityTestRule(MainActivity.class);
    ///////////////////////////////////////////////// Test case /////////////////////////
    @Test
    public void DENY_TESTS() {

        permission("Deny");

        clickTest(TOOL_BUTTON_INSTANCE);

        permission("Deny");
        waitfortime(2000);
        takess(6);
        istoolsmenuvisible();
        waitfortime(1000);
        clickTest(BACK_ARROW_INSTANCE);
        takess(7);
        clickTest(MAIN_APP_BUTTON_INSTANCE);
        waitfortime(1000);
        clickback();
        waitfortime(1000);
        permission("Deny");
        waitfortime(1000);
        clickTest(LIBRARY_BUTTON_INSTANCE);
        waitfortime(1000);
        clickback();
        waitfortime(1000);
        permission("Deny");


    }


    private void clickTest(int INSTANCE_NUMBER){ // click event

        try {

            UiDevice device = UiDevice.getInstance(getInstrumentation());
            UiObject allowPermissions = device.findObject(new UiSelector()
                    .clickable(true)
                    .checkable(false)
                    .instance(INSTANCE_NUMBER));
            if (allowPermissions.exists()) {
                allowPermissions.click();
            }else {
                Log.d("Instance","Does Not Exist");
            }

        } catch (UiObjectNotFoundException e) {

        }

    }
    private void waitfortime(int TIME){ // wait for mili second
        try {
            Thread.sleep(TIME);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }
    private void clickback(){ /// click mobiles back button
        UiDevice device = UiDevice.getInstance(getInstrumentation());
        device.pressBack();
    }

    private void ismainsmenuvisible(){
        onView(withId(R.id.button_todoschool)).check(matches(isDisplayed()));
        onView(withId(R.id.button_library)).check(matches(isDisplayed()));
        onView(withId(R.id.button_tool)).check(matches(isDisplayed()));
        onView(withId(R.id.textView_currentUserId)).check(matches(isDisplayed()));
        onView(withId(R.id.launcher_title_button)).check(matches(isDisplayed()));
        onView(withId(R.id.textView_numCoin)).check(matches(isDisplayed()));
        onView(withId(R.id.imageView_coin)).check(matches(isDisplayed()));
    }

    private void istoolsmenuvisible(){
        onView(withId(R.id.image_coin)).check(matches(isDisplayed()));
        onView(withId(R.id.textView_numCoin)).check(matches(isDisplayed()));
        onView(withId(R.id.app_drum)).check(matches(isDisplayed()));
        onView(withId(R.id.app_marimba)).check(matches(isDisplayed()));
        onView(withId(R.id.app_blackboard)).check(matches(isDisplayed()));
        onView(withId(R.id.app_coloring)).check(matches(isDisplayed()));
        onView(withId(R.id.app_drawing)).check(matches(isDisplayed()));
        onView(withId(R.id.app_album)).check(matches(isDisplayed()));
        onView(withId(R.id.app_fish_bowl)).check(matches(isDisplayed()));
        onView(withId(R.id.app_writing_board)).check(matches(isDisplayed()));
    }

    private void isvideomenuvisible(){
        onView(withId(R.id.installAppButton)).check(matches(isDisplayed()));
        onView(withId(R.id.videoCloseButton)).check(matches(isDisplayed()));
        onView(withId(R.id.videoSurface)).check(matches(isDisplayed()));
    }

    private void permission(String action) {
        if (Build.VERSION.SDK_INT >= 23) {
            UiObject allowPermissions = UiDevice.getInstance(getInstrumentation()).findObject(new UiSelector().text(action));
            if (allowPermissions.exists()) {
                try {
                    allowPermissions.click();
                } catch (UiObjectNotFoundException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private void takess(int i){    // takes screen shot of instance
        File path = new File("/storage/emulated/0/test-screenshots/filename" + Integer.toString(i));
        int SDK_VERSION = android.os.Build.VERSION.SDK_INT;
        if (SDK_VERSION >= 17) {
            waitfortime(1000);
            UiDevice.getInstance(getInstrumentation()).takeScreenshot(path);
            waitfortime(1000);
        }
    }
}

