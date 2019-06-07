# OpenCV のインポート
import cv2
import seed_net_wrap as gnw
import numpy as np

# VideoCaptureのインスタンスを作成する。
# 引数でカメラを選べれる。
cap = cv2.VideoCapture(0)
# Cpumpkin20181218_verylight network;

net = gnw.create()
gnw.initialize(net)
gnw.load_weights(net, "pumpkin20181218_verylight_weights.bin")
gnw.commit(net)

while True:
    # VideoCaptureから1フレーム読み込む
    ret, frame = cap.read()

    # スクリーンショットを撮りたい関係で1/4サイズに縮小
    # frame = cv2.resize(frame, (int(frame.shape[1]/4), int(frame.shape[0]/4)))
    # 加工なし画像を表示する
    cv2.imshow('Raw Frame', frame)
    img = np.ascontiguousarray(frame)

    # run network
    gnw.put_input(net, img)
    gnw.run_network(net)
    output = gnw.get_final_output(net)
    print(output)

    # 何か処理（ここでは文字列「hogehoge」を表示する）
    # edframe = frame
    # cv2.putText(edframe, 'hogehoge', (0,50), cv2.FONT_HERSHEY_PLAIN, 3, (0, 255,0), 3, cv2.LINE_AA)

    # 加工済の画像を表示する
    # cv2.imshow('Edited Frame', edframe)

    # キー入力を1ms待って、k が27（ESC）だったらBreakする
    k = cv2.waitKey(1)
    if k == 27:
        break

# キャプチャをリリースして、ウィンドウをすべて閉じる
cap.release()
cv2.destroyAllWindows()
