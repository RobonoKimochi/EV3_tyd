8月29日 島田

〇変更点
・LineTracerアプリにRunManagerアプリを実装して走行区間の判断、
  及び走行区間終了後に、ルックアップゲート等の難関区間用の
  アプリケーションに切り替える処理を追加いたしました。

・TeraTerm画面のUIにて、Logger開始後にMenuを表示しないよう微修正しました。

・Oddmetryユニットにて、以下の関数を削除し、切り分けをしました

旧:void Odmetry::getLocation(float& x, float& y);
新:float Odmetry::getX(){return mX;}
   float Odmetry::getY(){return mY;}

・RunManagerアプリにて、オドメトリを使って、原点の設定とそこからの距離を
  取得できる関数を下記のように追加しております。

   void setOrigin();               // 距離を測る時の原点を設定する
   float getDistanceFromOrigin();  // 原点からの距離を測定する

8月29日 山口

〇追加点
・難所ガレージのモジュールソース（Garage.Cpp、Garage.h）追加。

〇変更点
・LineTracerWithStarter.CppのStateにGARAGEを追加し
　ガレージ状態の際の処理関数を追加。

備考：正常に動作しない走行体のため、完全なテストを行えていない。
　　　GARAGEの停止位置までの走行距離は、適当な値を指定しているため
　　　調整を行う必要がある。
　　　
　