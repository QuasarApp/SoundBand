function Controller()
{
    installer.uninstallationFinished.connect(this, Controller.prototype.uninstallationFinished);
    installer.installationFinished.connect(this, Controller.prototype.installationFinished);
    runProgramControl();
}


function runProgramControl() {
    if (systemInfo.kernelType === "winnt") {
        installer.setValue("RunProgram", "@TargetDir@/SoundBand.exe");
    }
}

Controller.prototype.uninstallationFinished = function()
{

}


Controller.prototype.installationFinished = function()
{

}

