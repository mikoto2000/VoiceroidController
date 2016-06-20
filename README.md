VoiceroidController
===================

VOICEROID+ をコマンドプロンプトから操作するためのツールを目指しています。

Usage:
------

下記のような使い方を想定しています。

```bat
rem 「読み上げ音声」を再生
VoiceroidController.exe 読み上げ音声

rem 「読み上げ音声」を「OUTPUT_FILE.wav」に保存
VoiceroidController.exe -o OUTPUT_FILE.wav 読み上げ音声

rem 「文字列ファイル.txt」を再生
VoiceroidController.exe -i 文字列ファイル.txt

rem 「読み上げ音声」を「OUTPUT_FILE.wav」に保存(保存完了まで待機)
VoiceroidController.exe -s -o OUTPUT_FILE.wav 読み上げ音声
```

limitation:
-----------

読み上げに、「VOICEROID＋ 結月ゆかり」を使用する場合、
読み上げるたびにクリップボードが読み上げ文章で上書きされてしまいます。

Implemented:
------------

実装状況は下記の通りです。

- [ ] : VOICEROID 選択機能
- [x] : VOICEROID＋ 結月ゆかり EX
    - 音声再生
        - [x] : 入力ファイル指定
        - [x] : 同期モード
        - [x] : 非同期モード
    - 音声保存
        - [x] : 入力ファイル指定
        - [x] : 保存ファイル指定
        - [x] : 同期モード
        - [x] : 非同期モード
- [ ] : VOICEROID＋ 結月ゆかり
    - 音声再生
        - [x] : 入力ファイル指定
        - [x] : 同期モード
        - [x] : 非同期モード
    - 音声保存
        - [ ] : 入力ファイル指定
        - [ ] : 保存ファイル指定
        - [ ] : 同期モード
        - [ ] : 非同期モード

TODO:

- [ ] : 再生途中だったときの挙動を考える
- [x] : クラス分割
- [ ] : 長文ファイル指定時の自動分割を考える
- [ ] : クリップボード汚染対策


Requirements:
-------------

？？？

Install:
--------

？？？

License:
--------

Copyright (C) 2016 mikoto2000

This software is released under the MIT License, see LICENSE

このソフトウェアは MIT ライセンスの下で公開されています。 LICENSE を参照してください。

Author:
-------

mikoto2000 <mikoto2000@gmail.com>

