function clamp(x,a,b){
	return Math.max(a,Math.min(x,b));
}


function getRndInteger(min, max) {
  return Math.floor(Math.random() * (max - min) ) + min;
}

//get random vector in a edges (max-min) cube
function getRndVector(min,max){
    let x=getRndInteger(min,max);
    let y=getRndInteger(min,max);
    let z=getRndInteger(min,max);
    return new THREE.Vector3(x,y,z);
}

function getRndPositiveVector(){
    let x=Math.random();
    let y=Math.random();
    let z=Math.random();
    return new THREE.Vector3(x,y,z);
}

function rndSign(){
    if(Math.random()<0.5){
        return -1;
    }
    return 1;
}

function getRndVector(){
    let x=Math.random();
    let y=Math.random();
    let z=Math.random();

    return new THREE.Vector3(x*rndSign(),y*rndSign(),z*rndSign());
}
