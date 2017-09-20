

function acender(elem){
	
	var status = elem.value;

	if(status == "1"){
		elem.src="images/lampada-off.png";
		elem.value = "0";
	}else{
		elem.src="images/lampada-on.png";
		elem.value = "1";
	}
}

function ligar(elem){
	
	var status = elem.value;

	if(status == "1"){
		elem.src="images/switch-off.png";
		elem.value = "0";
	}else{
		elem.src="images/switch-on.png";
		elem.value = "1";
	}
}

function ligartomada(elem){
	
	var status = elem.value;

	if(status == "1"){
		elem.src="images/tomadaoff.png";
		elem.value = "0";
	}else{
		elem.src="images/tomadaon.png";
		elem.value = "1";
	}
}

