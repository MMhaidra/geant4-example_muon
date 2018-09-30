# README
Muography のための Geant4 example ソースファイル  
作成者：吉原
(2018.9.30 作成)

## 動作環境

- Mac OS High Sierra
- Geant4 10.04 (古いバージョンの Geant4 だと cmake が通らない可能性がある)
 
## 1. 必要ソフトウェアのインストール

#### ROOT 
以下のURLを参照して、インストールする。（ついでにPyROOTも使えるようにしておくと良い）
[http://osksn2.hep.sci.osaka-u.ac.jp/~taku/osx/install_root.html](http://osksn2.hep.sci.osaka-u.ac.jp/~taku/osx/install_root.html)

#### Geant4
以下のURLを参照して、インストールする。  
[https://qiita.com/takenawa/items/3158afc19aaaffd767c5](https://qiita.com/takenawa/items/3158afc19aaaffd767c5)
 
## 2. ソースファイルのビルド
まず、ソースファイルをダウンロードする。Terminal で下のコマンドを打つ。

```
$ git clone https://github.com/yoshihara-yuli/geant4-example_muon
```
上のコマンドでエラーが出た人は、brew で事前に git をインストールする。(```brew install git```)

次に、ダウンロードしたディレクトリに移動して、ビルド用ファイル(build/)を作成して、そこでソースファイルをcmake(gcmake)してから、ビルド(make)する。

```
$ cd geant4-example_muon
$ mkdir build
$ cd build
$ gcmake ../source
$ make -j4
```

gcmake は、Geant4 インストールの参考 URL を元に設定する。
（あるいは、普通に cmake をしても良い。)

そうすると、実行ファイル ./mu が生成される。

実行ファイルを実行する前に、**bench/ に入っているマクロファイルを build/ 下にコピーしてくる。**

```
$ cp ../bench/* .
$ ./mu
```

これで、GUI（Qt）が立ち上がる。


### 注意事項

- source/の中にあるファイルに変更を加えたら、必ず make -j4 をする(そうでなければ実行ファイルが更新されない)
- source/の中にあるファイルの削除やファイル名の変更、あるいは新規ファイルの追加を行なった場合は、build/ の中身をすべて削除したのち、再び cmake(gcmake)から行う。

## 3. シミュレーションの方法

### A: テストモード

ジオメトリ(検出器など)やソース(線源)の確認のために、GUI(Qt)を立ち上げてチェックする。

まず、ビルドによって生成された実行ファイルを実行する。

```
./mu
```
これで、下のような画面が表示される。 定義したジオメトリを確認することができる。
（図の青色がシンチレータ、白色が筐体を模擬したモジュール(今は空気)。）

![geometry_for_muography](geometry.png)


次に、GUIの下の方にある**Output**のコンソールで、実行用のマクロファイル(run.mac)を実行する。

```Outout
/control/execute run.mac
```
すると、run.mac に指定してあるビーム数(/run/beamOn)だけ、ミューオン(赤色)が生成される。これで、ソース(線源)の定義も確認することができる。

### B: バッチモード

ジオメトリなどが正しく定義してあれば、あとはシミュレーションをたくさん回すだけ。  
その時は、Terminal で実行用マクロファイルを以下のように実行する。

```
$ ./mu run.mac > /dev/null 2>&1
```
**\> /dev/null 2>&1** で、Terminal に本来表示されるべき文字をゴミ箱に移している（はず）。これにより、一般的な実行(```$ ./mu run.mac```)より早くシミュレーションが回せる。


## 3. 出力ファイル

build/ 下に、**out.root** として出力される。(ROOTのデータ形式)

### 出力データの確認

Terminal で ROOT を起動させ、データのツリー構造にアクセスする。

```
$ root out.root
root [1] new TBrowser;
```

下の画面(ROOT)が起動するので、左側のROOT Files > out::root をダブルクリックし、その下に現れるtree::1をダブルクリックすると、左メニューに**nHit, x, y, z, time, eIn, eDep, TrackID, copyNo, particle**が表示されます。

その中から見たい項目をさらにダブルクリックすると、右側にそのヒストグラムが表示されます。（図は、edep (各シンチに落としたエネルギー)のヒストグラム)

![root_tbrowser](root.png)

他の項目に関しては、出力データの解析ファイルができたら、まとめたいと思います。（それか、ソースファイルを読んでもらってもわかると思います）

### 出力データの解析

解析には、ROOT が必要だが、PyROOTを使えば、Python上で、ROOTファイルを読み込んでそのままデータを解析できる。(まだ作ってないので今度作る)


## 4. ジオメトリやソース(線源)の変更

[source/AboutCode.md](https://github.com/yoshihara-yuli/geant4-example_muon/blob/master/source/AboutCode.md) を参照。

## 5. その他

このコードは開発途上である(実装できてない箇所がある)-> 詳細は [やり残し作業](https://github.com/yoshihara-yuli/geant4-example_muon/blob/master/geant4_todo.md)

質問 --> yoshihara.yuli at gmail.com