var DIRS=createDirections(30,1.61803398875);

function createDirections(num,turnFraction){
    var dirs=[];
    for(i=0;i<num;i++){
        let dst=i/(num-1.0);
        let inclination=Math.acos(1-2*dst);
        //let inclination=viewAngle/2;
        let azimuth=2.0*Math.PI*turnFraction*i;

        let x=Math.sin(inclination)*Math.cos(azimuth);
        let y=Math.sin(inclination)*Math.sin(azimuth);
        let z=Math.cos(inclination);

        dirs.push(new THREE.Vector3(x,y,z));
    }
    return dirs;
}

class Fish{
	constructor(scene,id,fishMesh,materialShaders){
		this.mesh=fishMesh.clone();
		this.mesh.name=id.toString();
		this.materialShaders=materialShaders;

		//this.mesh.castShadow=true;
	
		this.offset=Math.random()*10;

		this.id=id;
		this.scene=scene;

		this.velocity=getRndVector();
		this.acceleration=new THREE.Vector3();
		this.rotation=new THREE.Matrix4();


		this.position=getRndVector().multiplyScalar(settings.length/2);
        this.mesh.position.x+=this.position.x;
        this.mesh.position.y+=this.position.y;
        this.mesh.position.z+=this.position.z;

		//this.mesh.lookAt(this.velocity);
		this.mesh.lookAt(this.position.clone().add(this.velocity));

		this.ray=new THREE.Ray(this.position.clone(),this.velocity.clone());
		
		this.scene.add(this.mesh);
	}

	update(time,obstacles,localForce){
		if(this.materialShaders){
			for(let i=0;i<this.materialShaders.length;i++){
				if(this.materialShaders[i]){
					this.materialShaders[i].uniforms.time={value:time+this.offset}
				}
			}
		}

		this.acceleration.set(0,0,0);

		this.rotation.extractRotation(this.mesh.matrixWorld);

		this.ray.origin=this.position.clone();
		this.ray.direction=this.velocity.clone();

		this.acceleration.add(localForce);

        if(this.isBlocked(obstacles)){
            let newdir=this.findMovedir(obstacles);
            if(newdir){
                this.acceleration=newdir.clone().multiplyScalar(settings.max_speed);
            }
            else{
                console.error("OH OH, SOMETHING WRONG HAPPENED, NO NEW DIRECTION FOUND");
            }
        }

		this.velocity.add(this.acceleration);
		let speed=this.velocity.length();
		let dir=this.velocity.divideScalar(speed);
		speed=clamp(speed,settings.min_speed,settings.max_speed);
		this.velocity.multiplyScalar(speed);

		this.mesh.lookAt(this.position.clone().add(this.velocity));
		
		this.mesh.position.x+=this.velocity.x;
        this.mesh.position.y+=this.velocity.y;
        this.mesh.position.z+=this.velocity.z;

		this.position=this.mesh.position;
	}

	//TODO: placeholder vectors! to avoid repeatedly creating vectors
    isBlocked(obs){
        for(let j=0;j<obs.length;j++){
            let res=new THREE.Vector3();
            if(this.ray.intersectBox(obs[j],res)){
                let distance=this.position.clone().sub(res).length();
                if(distance<=settings.max_collision_dist){
                    return true;
                }
            }
        }
        return false;
    }

	findMovedir(obs){
        let newdirs=DIRS.filter(function(v){return v.z>=Math.cos(settings.view_angle/2);});
        for(i=0;i<newdirs.length;i++){
            let d=newdirs[i].clone();
            d.applyMatrix4(this.rotation);
            this.ray.direction=d;

            if(!this.isBlocked(obs)){
                return d;
            }
        }
        return;
    }

	steerTowards(vector){
		let v=vector.normalize().multiplyScalar(settings.max_speed).sub(this.velocity);
		return v.clampLength(0,settings.max_steer_force);
	}
}
