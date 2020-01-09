class Settings{
	constructor(){
		this.length=500;
		this.fish=100;
	
		this.min_speed=.5;
		this.max_speed=1.0;

		this.perception_radius=50;
		this.align_weight=.1;
		this.cohesion_weight=.1;
		this.separation_weight=.1;

		this.max_steer_force=3;

		this.max_collision_dist=10;

		this.view_angle=(3/2)*Math.PI;
	}
}
