#include <vector>
#include <unordered_map>
#include <M5StickC.h>
#include <BleKeyboard.h>

// -----------------------------------------------------------------------------
// 定数
// -----------------------------------------------------------------------------
// 電源ボタンが1秒未満押された
const uint8_t AXP_WAS_PRESSED = 2;
// キーと待ち時間の構造体
struct Key {
  uint8_t key;
  uint32_t delay;
};
// 送信するキーストローク
const std::vector<Key> keystroke = {
    { 'e'           , 1000 },   // エクスキューブ交換ショップ店員に話しかける
    { KEY_RETURN    , 500 },    // ショップメニューで「エクスキューブ交換ショップ」を選択
    { KEY_LEFT_ARROW, 300 },    // 「グラインダー(30個)」を最大数に設定
    { KEY_RETURN    , 300 },    // 決定
    { KEY_RETURN    , 500 },    // 交換確認画面で「はい」を選択
    { KEY_RETURN    , 300 },    // 交換に成功したメッセージを閉じる
    { KEY_ESC       , 300 },    // 交換リストを閉じる
    { KEY_ESC       , 300 },    // ショップメニューを閉じる
    { KEY_HOME      , 300 },    // メインメニューを開く
    { KEY_RETURN    , 300 },    // 「アイテム」を選択
    { KEY_LEFT_SHIFT, 300 },    // グラインダーを全数選択
    { KEY_RETURN    , 300 },    // 決定
    { KEY_RETURN    , 300 },    // 「倉庫に預ける」を選択
    { KEY_DOWN_ARROW, 300 },    // 「キャラ倉庫に預ける」へカーソルを移動
    { KEY_RETURN    , 500 },    // 決定
    { KEY_HOME      , 0 }       // メインメニューを閉じる
};

// -----------------------------------------------------------------------------
// 変数
// -----------------------------------------------------------------------------
// ESP32-BLE-Keyboard
BleKeyboard bleKeyboard;

// -----------------------------------------------------------------------------
// 関数
// -----------------------------------------------------------------------------
// 画面を消去する
void clearScreen() {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setCursor(0, 0);
}

// キーストロークを送信する
void sendKeystroke() {
    clearScreen();
    M5.Lcd.println("Sending...");

    for (const auto& n : keystroke) {
        bleKeyboard.write(n.key);
        delay(n.delay);
    }

    M5.Lcd.println("Complete!");
}

void setup() {
    M5.begin();

    // 画面の表示設定
    M5.Lcd.setRotation(1);
    M5.Lcd.setTextFont(2);
    clearScreen();
    M5.Lcd.println("pso2-excube2grinder");

    // Bluetooth接続
    bleKeyboard.begin();
}

void loop() {
    M5.update();

    // 電源ボタンが押されたらリセット
    if (M5.Axp.GetBtnPress() == AXP_WAS_PRESSED) {
        esp_restart();
    }

    if (bleKeyboard.isConnected()) {
        // Bluetooth接続済みの場合
        // ボタンAが押されたらキーを送信
        if (M5.BtnA.wasPressed()) {
            sendKeystroke();
        }

        delay(10);
    }
    else {
        // Bluetooth未接続の場合は5秒待つ
        M5.Lcd.println("Waiting 5 seconds...");
        delay(5000);
    }
}
