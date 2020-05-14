//-----------------------------------------------------
// This is my second Verilog Design
// Design Name : first_counter
// File Name : first_counter.v
// Function : This is a 4 bit up-counter with
// Synchronous active high reset and
// with active high enable signal
//-----------------------------------------------------
module Jump_Duck_Dino (
	GPILDR1,
	GPILDR2,
//	GPILDR3,
	LEDG,
	SPACEBAR,
	DOWNKEY,
	CLOCK_50
);

	output wire [7:0] LEDG;
	output wire SPACEBAR;
	output wire DOWNKEY;
	input wire GPILDR1;
	input wire GPILDR2;
//	input wire GPILDR3;
	input wire CLOCK_50;

	// reg
	reg [7:0] rLEDG;
	reg rSPACEBAR;
	reg rDOWNKEY;

	// assign
	assign LEDG = rLEDG;
	assign SPACEBAR = rSPACEBAR;
	assign DOWNKEY = rDOWNKEY;

	// always
	always @(posedge CLOCK_50) begin
		if (GPILDR1 == 1'b1) begin
			rSPACEBAR <= 1'b1;            	// Spacebar pressed
			rLEDG[0] <= 1'b1;       			// First LED turned on
		end 
		else begin
			rSPACEBAR <= 1'b0;            	// Spacebar released
			rLEDG[0] <= 1'b0;						// First LED turned off
		end
		
		if (GPILDR2 == 1'b1) begin
			rDOWNKEY <= 1'b1;       			// Down key pressed
			rLEDG[1] <= 1'b1;       			// Second LED turned on
		end 
//		else if (GPILDR3 == 1'b1) begin
//			rLEDG <= 1'b1 << 2;       // LED turned on
//		end 
		else begin
			rDOWNKEY <= 1'b0;            		// Down key released
			rLEDG[1] <= 1'b0;       			// Second LED turned off
		end
	end
 
endmodule 