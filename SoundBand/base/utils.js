function dp(pixelDensity,x) {
	return (pixelDensity * 25.4 < 120) ? x : x*(pixelDensity * 25.4/160);
}

function baseColor() {
	return "#f26e35"
}

function primaryColor() {
	return "#d04828"
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
	return "#ffefde"
}

