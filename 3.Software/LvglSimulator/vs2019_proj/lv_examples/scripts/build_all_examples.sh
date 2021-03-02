#!/bin/bash
set -e
mkdir -p objs
cd objs
echo "-- Updating builder ..."
OBJS_DIR=`pwd`
if ! [ -d builder ]; then
  git clone --quiet --recursive https://github.com/lvgl/lv_sim_emscripten.git builder
  cd builder
else
  cd builder
  git pull --quiet
fi
cd lv_examples
echo "-- Checking out lv_examples $GITHUB_SHA ..."
git checkout $GITHUB_SHA
cd ..
echo "<html><body><h1>Choose an example:</h1><ul>" > $OBJS_DIR/index.html
cat $OBJS_DIR/../scripts/examples.txt | while read -r example_name; do
  echo "-- Building $example_name ..."
  make -j4 CHOSEN_DEMO=$example_name NO_GIT_HASH=1 || exit 1
  rm -rf $OBJS_DIR/$example_name
  cp -a build $OBJS_DIR/$example_name
  echo "<li><a href=\"$example_name/index.html\">$example_name</a></li>" >> $OBJS_DIR/index.html
done
echo "</ul></body></html>" >> $OBJS_DIR/index.html
