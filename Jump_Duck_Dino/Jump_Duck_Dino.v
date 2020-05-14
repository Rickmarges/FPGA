//-----------------------------------------------------
// This Verilog design is used in an FPGA project to 
// play the Google Chrome's Dino Jump game
// Design Name : Jump_Duck_Dino
// File Name : Jump_Duck_Dino.v
// Authors: Maryn Kaspers & Rick Margés
//-----------------------------------------------------
module Jump_Duck_Dino (
	GPILDR1,		// The input pin for the background LDR
	GPILDR2,		// The input pin for the first foreground LDR
	GPILDR3,		// The input pin for the second foreground LDR
	LEDG,			// Build in green LED for testing purpose
	SPACEBAR,	// The output pin for the spacebar keystroke
	DOWNKEY,		// The output pin for the down keystroke
	CLOCK_50	
);

	// Outputs
	output wire [7:0] LEDG;
	output wire SPACEBAR;
	output wire DOWNKEY;
	
	// Inputs
	input wire GPILDR1;
	input wire GPILDR2;
	input wire GPILDR3;
	input wire CLOCK_50;

	// reg
	reg [7:0] rLEDG;
	reg rSPACEBAR;
	reg rDOWNKEY;

	// assign
	assign LEDG = rLEDG;
	assign SPACEBAR = rSPACEBAR;
	assign DOWNKEY = rDOWNKEY;

	// At every positive edge of the clock cycle
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