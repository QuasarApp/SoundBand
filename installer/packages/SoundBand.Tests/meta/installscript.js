// Constructor
function Component()
{

    generateTr();
}

function generateTr() {
    component.setValue("DisplayName", qsTr("SoundBandTests"));
    component.setValue("Description", qsTr("Install tests of SoundBand"));

}

function isNoEmpty(dir){
    var libsArray = installer.findPath("*", [dir]);
    return Boolean(libsArray.length);
}
