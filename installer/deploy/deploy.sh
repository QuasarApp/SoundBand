#!/bin/bash

CURRENTDIR=$(dirname "$(readlink -f "$0")")

CQtDeployer="$CURRENTDIR/../../CQtDeployer"

deploy="$CQtDeployer/build/release/cqtdeployer.sh"
qmldir=$1
qmake=$2

chmod +x $CQtDeployer/sharedBuild.sh

$CQtDeployer/sharedBuild.sh $qmake

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

$deploy -qmake "$qmake"  clear -qmlDir "$qmldir" -bin "$CURRENTDIR/../packages/SoundBand/data/SoundBand"


