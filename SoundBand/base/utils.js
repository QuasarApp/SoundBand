function dp(pixelDensity,x) {
	return (pixelDensity * 25.4 < 120) ? x : x*(pixelDensity * 25.4/160);
}

function baseColor() {
    return "#303030"
}

function primaryColor() {
    return "#acacac"
}

function textColor() {
	return "#434f4d"
}

function textAltColor() {
	return "#a6aaa2"
}

function backgroundColor() {
    return "#444444"
}

function backgroundAltColor() {
    return "#333333"
}

