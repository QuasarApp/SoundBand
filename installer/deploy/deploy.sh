#!/bin/bash

CURRENTDIR=$(dirname "$(readlink -f "$0")")

deploy="$CURRENTDIR/../../CQtDeployer/build/release/cqtdeployer"
qmldir=$1
qmake=$2

rm -rdf "$CURRENTDIR/../packages/SoundBand/data"
rm -rdf "$CURRENTDIR/../packages/SoundBand.Tests/data"

mkdir "$CURRENTDIR/../packages/SoundBand/data"
mkdir "$CURRENTDIR/../packages/SoundBand.Tests/data"

cp -R "$CURRENTDIR/../packages/SoundBand/extra/." "$CURRENTDIR/../packages/SoundBand/data"


cp -Rp "$CURRENTDIR/../../SoundBand/build/release/SoundBand" "$CURRENTDIR/../packages/SoundBand/data/"
cp -Rpf "$CURRENTDIR/../../QuasarAppLib/build/release/." "$CURRENTDIR/../packages/SoundBand/data"
cp -Rpf "$CURRENTDIR/../../Sync/build/release/." "$CURRENTDIR/../packages/SoundBand/data/"
cp -Rp "$CURRENTDIR/../../tests/build/release/test" "$CURRENTDIR/../packages/SoundBand.Tests/data/"

chmod +x $deploy

cp -Rpf "$CURRENTDIR/../../CQtDeployer/QuasarAppLib/build/release/." "$CURRENTDIR/../../CQtDeployer/build/release"

$deploy -qmake $qmake  clear -qmldir $qmldir -bin "$CURRENTDIR/../packages/SoundBand/data/SoundBand"


