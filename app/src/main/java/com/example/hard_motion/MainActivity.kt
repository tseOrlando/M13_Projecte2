package imgui.example.android

import android.app.NativeActivity
import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.KeyEvent
import android.view.inputmethod.InputMethodManager
import java.util.concurrent.LinkedBlockingQueue


class MainActivity : NativeActivity()
{
    init { System.loadLibrary("ImGuiExample") }

    public override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState)
    }

    private external fun handleBackPressed()

    @Override
    override fun onKeyDown(keyCode: Int, event: KeyEvent?): Boolean
    {
        if (keyCode == KeyEvent.KEYCODE_BACK && event?.action == KeyEvent.ACTION_DOWN)
        {
            handleBackPressed()
            return false
        }

        return super.onKeyDown(keyCode, event)
    }

    fun showSoftInput()
    {
        val inputMethodManager = getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
        inputMethodManager.showSoftInput(this.window.decorView, 0)
    }

    fun hideSoftInput()
    {
        val inputMethodManager = getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
        inputMethodManager.hideSoftInputFromWindow(this.window.decorView.windowToken, 0)
    }

    // Queue for the Unicode characters to be polled from native code (via pollUnicodeChar())
    private var unicodeCharacterQueue: LinkedBlockingQueue<Int> = LinkedBlockingQueue()

    // We assume dispatchKeyEvent() of the NativeActivity is actually called for every
    // KeyEvent and not consumed by any View before it reaches here
    override fun dispatchKeyEvent(event: KeyEvent): Boolean
    {
        if (event.action == KeyEvent.ACTION_DOWN)
            unicodeCharacterQueue.offer(event.getUnicodeChar(event.metaState))

        return super.dispatchKeyEvent(event)
    }

    fun pollUnicodeChar(): Int
    {
        return unicodeCharacterQueue.poll() ?: 0
    }
}
