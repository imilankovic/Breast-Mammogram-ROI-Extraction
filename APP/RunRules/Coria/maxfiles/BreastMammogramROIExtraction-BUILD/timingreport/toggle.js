function toggle(showHideDiv, switchImgTag) {
	var ele = document.getElementById(showHideDiv);
	var txt = document.getElementById(switchImgTag);
	if(ele.style.display == "block") {
		ele.style.display = "none";
		txt.innerHTML = '<img src="images/plus.png"></img>';
	} else {
		ele.style.display = "block";
		txt.innerHTML = '<img src="images/minus.png"></img>';
	}
}
