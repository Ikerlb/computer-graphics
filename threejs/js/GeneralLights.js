function GeneralLights(scene,x,y,z) {
	
	const light = new THREE.DirectionalLight("101010", 1);
	light.position=new THREE.Vector3(0,1,0);
	//light.castShadow=true;

	scene.add(light);


	
	this.update = function(time) {
		//light.intensity = (Math.sin(time)+1.5)/1.5;
		//light.color.setHSL( Math.sin(time), 0.5, 0.5 );
	}
}
