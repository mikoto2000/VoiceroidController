VoiceroidController
===================

VOICEROID+ をコマンドプロンプトから操作するためのツールを目指しています。

Usage:
------

```bat
Usage: VoiceroidController.exe [options] [text]
オプション:
  -h [ --help ]               ヘルプを表示
  --voiceroid arg (=YukariEx) 読み上げ VOICEROID(Yukari, YukariEx)
  -o [ --output-file ] arg    出力ファイルパス
  -i [ --input-file ] arg     入力ファイルパス
  -u [ --utf8 ]               入力ファイル文字コードを UTF8 として処理
  -s [ --sync ]               同期モード(再生・保存が完了するまで待機します)
  --split-size arg (=20000)   読み上げ文字列を分割する目安のサイズ
```

下記のような使い方を想定しています。

```bat
rem 「読み上げ音声」を再生
VoiceroidController.exe 読み上げ音声

rem 「VOICEROID＋ 結月ゆかり」で「読み上げ音声」を再生
VoiceroidController.exe --voiceroid Yukari 読み上げ音声

rem 「読み上げ音声」を「OUTPUT_FILE.wav」に保存
VoiceroidController.exe -o OUTPUT_FILE.wav 読み上げ音声

rem 「文字列ファイル.txt」を再生
VoiceroidController.exe -i 文字列ファイル.txt

rem 「文字列ファイル.txt」を再生(ファイルが UTF8 のテキストの場合)
VoiceroidController.exe -u -i 文字列ファイル.txt

rem 「読み上げ音声」を「OUTPUT_FILE.wav」に保存(保存完了まで待機)
VoiceroidController.exe -s -o OUTPUT_FILE.wav 読み上げ音声

rem 「文字列ファイル.txt」を「OUTPUT_FILE_xxx.wav」に保存(xxx は 000 からの連番)
rem 1 ファイル毎の読み上げ文字列が 100 文字を超えないように分割してファイル出力する
VoiceroidController.exe --split-size 100 -s -i 文字列ファイル.txt -o OUTPUT_FILE.wav 読み上げ音声
```

limitation:
-----------

読み上げに、「VOICEROID＋ 結月ゆかり」を使用する場合、
読み上げるたびにクリップボードが読み上げ文章で上書きされてしまいます。

Implemented:
------------

実装状況は下記の通りです。

- [x] : VOICEROID 選択機能
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
- [x] : VOICEROID＋ 結月ゆかり
    - 音声再生
        - [x] : 入力ファイル指定
        - [x] : 同期モード
        - [x] : 非同期モード
    - 音声保存
        - [x] : 入力ファイル指定
        - [x] : 保存ファイル指定
        - [x] : 同期モード
        - [x] : 非同期モード

TODO:

- [ ] : 再生途中だったときの挙動を考える
- [x] : クラス分割
- [x] : 長文ファイル指定時の自動分割を考える
    - [ ] : 自動分割時に sync モードになるようにする
    - [ ] : 自動分割時の連番書式指定ができるようにする
    - [ ] : 自動分割しない場合に連番を入れないようにする
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

