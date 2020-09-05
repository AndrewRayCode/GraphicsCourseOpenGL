- brew install assimp
- Xcode > Project > Build Settings tab > Library Search Paths > add "/usr/local/Cellar/assimp/version/lib/" to build and release search paths
- Click on the "build phases" tab
- Open "Link Binary With Libraries"
- Click the "+"
- On the bottom click "Add Other" > "Add Files..."
- Add the file /usr/local/Cellar/assimp/VERSION/lib/libassimp.VERSION.dylib

Also put in answer https://stackoverflow.com/a/63750631/743464
