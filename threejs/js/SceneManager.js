var gui = new dat.GUI()
var settings=new Settings();

gui.add(settings,"length");
gui.add(settings,"fish");

gui.add(settings,"perception_radius");

gui.add(settings,"align_weight");
gui.add(settings,"separation_weight");
gui.add(settings,"cohesion_weight");

gui.add(settings,"max_speed");
gui.add(settings,"min_speed");
gui.add(settings,"max_collision_dist");

function SceneManager(canvas) {

	const clock = new THREE.Clock();
    
	const screenDimensions = {
		width: canvas.width,
		height: canvas.height
	}
    var fish;
	
	const scene = buildScene();
	const flockingData=createFlockingData();
	const renderer = buildRender(screenDimensions);
	const camera = buildCamera(screenDimensions);
	const sceneSubjects = createSceneSubjects(scene);
	const obstacles=createObstacles(scene,settings.length,settings.length,settings.length,true);
	const cameraControls=createCameraControls(camera,renderer.domElement);
	//const fish = createFish(scene,1,LENGTH);
	//const walls=createWalls();

	function createObstacles(scene,width,height,depth,floor){
        var geometry;
        var material=new THREE.MeshBasicMaterial( {wireframe:true,color: 0xffff00});
        var mesh;

        var boxes=[];
        var obs=[];

        //create walls first
        //xy planes
        geometry=new THREE.PlaneGeometry(width,height);
        var xy1=new THREE.Mesh(geometry,material);
        var xy2=new THREE.Mesh(geometry,material);
        xy1.position.z=depth/2;
        xy2.position.z=-depth/2;
        obs.push(xy1);
        obs.push(xy2);

        //yz planes which means we rotate by y 90
        geometry=new THREE.PlaneGeometry(height,depth);
        var yz1=new THREE.Mesh(geometry,material);
        var yz2=new THREE.Mesh(geometry,material);
        yz1.rotation.y+=(Math.PI/2);
        yz2.rotation.y+=(Math.PI/2);
        yz1.position.x=width/2;
        yz2.position.x=-width/2;
        obs.push(yz1);
        obs.push(yz2);

        //zx which means we rotate by x 90
        geometry=new THREE.PlaneGeometry(width,depth);
        var xz1=new THREE.Mesh(geometry,material);
        var xz2=new THREE.Mesh(geometry,material);
        xz1.rotation.x+=(Math.PI/2);
        xz2.rotation.x+=(Math.PI/2);
        xz1.position.y=height/2;
        xz2.position.y=-height/2;
        obs.push(xz1);
        obs.push(xz2);

        for(i=0;i<obs.length;i++){
            let box=new THREE.Box3().setFromObject(obs[i]);
            boxes.push(box);
        }

		if(floor){
			var box=new THREE.BoxGeometry(width,height,depth);
			var mesh= new THREE.Mesh(box,material);
			var helper = new THREE.BoxHelper(mesh, 0xffff00 );
			scene.add(helper);
		}

        return boxes;
	}

	function createTerrain(){
		
	}

	function createCameraControls(camera,canvas){
		const cameraControls=new THREE.TrackballControls(camera,canvas);
		cameraControls.target.set(0,-settings.length/2+100,0);

		//camera.position.set( 0, 20, 100 );
		//camera.position.set(0,-settings.length+100,settings.length-100);
		camera.position.set(0,-settings.length/2+20,settings.length/2-20);
		//camera.lookAt(0,0,0);
		cameraControls.update();

		return cameraControls;
	}

	function buildScene() {
		const scene = new THREE.Scene();
		let fogColor=new THREE.Color("#07025F");
		scene.background = fogColor;
		//scene.fog=new THREE.Fog(fogColor,0.0025,settings.length);
		

		return scene;
	}

	function buildRender({ width, height }) {
		const renderer = new THREE.WebGLRenderer({ canvas: canvas, antialias: true, alpha: true }); 
		const DPR = (window.devicePixelRatio) ? window.devicePixelRatio : 1;
		renderer.shadowMap.enabled = false;
		//renderer.shadowMap.type = THREE.PCFSoftShadowMap;
		renderer.setPixelRatio(DPR);
		renderer.setSize(width, height);

		renderer.gammaInput = true;
		renderer.gammaOutput = true; 

		return renderer;
	}

    function buildCamera({ width, height }) {
		const aspectRatio = width / height;
		const fieldOfView = 60;
		const nearPlane = -10;
		const farPlane = 10; 
		const camera = new THREE.PerspectiveCamera(fieldOfView, aspectRatio, nearPlane, farPlane);

		return camera;
	}

	function createSceneSubjects(scene) {

		//TODO: terrain
		const sceneSubjects = [
			new GeneralLights(scene,50, 50, 50),
		];

		return sceneSubjects;
	}

	this.loadFishMesh=function(path,render){
		let mtlLoader=new THREE.MTLLoader();
        var mesh;
		var materialShaders=[];
		mtlLoader.load(
            path+".mtl",
            function(materials){
                var loader=new THREE.OBJLoader();
                loader.setMaterials(materials);
                loader.load(
                    path+".obj",
                    function(object){
                        mesh=object.children[0];
                        for(let i=0;i<object.children[0].material.length;i++){
                            object.children[0].material[i].onBeforeCompile=function(shader){
                                shader.uniforms.time={value:0};
                                shader.vertexShader='uniform float time;\n' + shader.vertexShader;
                                shader.vertexShader=shader.vertexShader.replace(
                                    '#include <begin_vertex>',
                                    [
                                        'vec3 transformed;',
                                        'vec3 newvector=vec3(position);',
                                        'newvector.x=newvector.x+cos(time);',
                                        'float pivot_angle=cos(time)*0.1*1.5;',
                                        'mat2 rotation_matrix=mat2(vec2(cos(pivot_angle), -sin(pivot_angle)), vec2(sin(pivot_angle), cos(pivot_angle)));',
                                        'newvector.xz=rotation_matrix*newvector.xz;',
                                        'float body=(newvector.z+1.0)/2.0;',
                                        'newvector.x=newvector.x+cos(time+body)*1.5;',
                                        'transformed=newvector;'
                                    ].join('\n')
                                );
                                materialShaders.push(shader);
                            }
                        }
						fish=createFish(scene,mesh,materialShaders);
						render();
                    }
                )

				//return createFish(scene,n,length,mesh,materialShaders);
			},
            function(xhr){
                console.log(xhr.loaded/xhr.total*100+'% loaded');
            },
            function(error){
                console.log('An error ocurred while loading '+path);
            }
			
        );
	}

	function createFish(scene,mesh,materials){
		f=[];
		for(let i=0;i<settings.fish;i++){
			f.push(new Fish(scene,i,mesh,materials));
		}
		return f;
	}

	function createFlockingData(){
		f=[];
		for(let i=0;i<settings.fish;i++){
			f.push(new THREE.Vector3());
		}
		return f;
	}

	this.updateFlockingData=function(){
		let align=new THREE.Vector3();
		let cohesion=new THREE.Vector3();
		let separation=new THREE.Vector3();
		let diff=new THREE.Vector3();
		let total=0;
        for(let i=0;i<fish.length;i++){
            align.set(0,0,0);
            cohesion.set(0,0,0);
            separation.set(0,0,0);
            flockingData[i].set(0,0,0);
            total=0;
            for(let other of fish){
                let sqdist=fish[i].position.distanceToSquared(other.position);
                if(other!=fish[i]&&sqdist<=Math.pow(settings.perception_radius,2)){
                    align.add(other.velocity);
                    cohesion.add(other.position);
                    diff.subVectors(fish[i].position,other.position).divideScalar(sqdist);
                    separation.add(diff);

                    total++;
                }
            }
            if(total>0){
                align.divideScalar(total);
                cohesion.divideScalar(total);
                separation.divideScalar(total);
                cohesion.sub(fish[i].position);


                let alignSteer=fish[i].steerTowards(align).multiplyScalar(settings.align_weight);
                flockingData[i].add(alignSteer);
                let cohesionSteer=fish[i].steerTowards(cohesion).multiplyScalar(settings.cohesion_weight);
                flockingData[i].add(cohesionSteer);
                let separationSteer=fish[i].steerTowards(separation).multiplyScalar(settings.separation_weight);
                flockingData[i].add(separationSteer);
            }
        }
	}

	//TODO: update fish
	this.update = function() {
		const elapsedTime = clock.getElapsedTime()*5;

		var delta = clock.getDelta();
		cameraControls.update(delta);
		
		for(let i=0; i<sceneSubjects.length; i++)
			sceneSubjects[i].update(elapsedTime);

		this.updateFlockingData();

		for(let i=0;i<fish.length;i++)
			fish[i].update(elapsedTime,obstacles,flockingData[i]);

		renderer.render(scene, camera);
	}


    this.onWindowResize = function() {
		const { width, height } = canvas;

		screenDimensions.width = width;
		screenDimensions.height = height;

		camera.aspect = width / height;
		camera.updateProjectionMatrix();

		renderer.setSize(width, height);
	}

	this.onDocumentMouseWheel=function(deltaY){
		camera.position.y -= deltaY;
		//prevent scrolling beyond a min/max value
		camera.position.clampScalar(-settings.length,settings.length);
	}

}
