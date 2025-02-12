(G-CODE GENERATED BY FLATCAM v8.994 - www.flatcam.org - Version Date: 2020/11/7)

(Name: global_vu-PTH.drl_cnc)
(Type: G-code from Geometry)
(Units: MM)

(Created on Monday, 12 August 2024 at 19:51)

(This preprocessor is used with a motion controller loaded with GRBL firmware.)
(It is configured to be compatible with almost any version of GRBL firmware.)


(TOOLS DIAMETER: )
(Tool: 1 -> Dia: 0.3)
(Tool: 2 -> Dia: 0.9)
(Tool: 3 -> Dia: 1.0)

(FEEDRATE Z: )
(Tool: 1 -> Feedrate: 30.0)
(Tool: 2 -> Feedrate: 30.0)
(Tool: 3 -> Feedrate: 30.0)

(FEEDRATE RAPIDS: )
(Tool: 1 -> Feedrate Rapids: 1500)
(Tool: 2 -> Feedrate Rapids: 1500)
(Tool: 3 -> Feedrate Rapids: 1500)

(Z_CUT: )
(Tool: 1 -> Z_Cut: -1.8)
(Tool: 2 -> Z_Cut: -1.8)
(Tool: 3 -> Z_Cut: -1.8)

(Tools Offset: )
(Tool: 3 -> Offset Z: 0.0)

(DEPTH_PER_CUT: )
(Tool: 1 -> DeptPerCut: 0.6)
(Tool: 2 -> DeptPerCut: 0.6)
(Tool: 3 -> DeptPerCut: 0.6)

(Z_MOVE: )
(Tool: 1 -> Z_Move: 2)
(Tool: 2 -> Z_Move: 2)
(Tool: 3 -> Z_Move: 2)

(Z Toolchange: 7.0 mm)
(X,Y Toolchange: 0.0000, 0.0000 mm)
(Z Start: None mm)
(Z End: 7.0 mm)
(X,Y End: None mm)
(Steps per circle: 64)
(Steps per circle: 64)
(Preprocessor Excellon: GRBL_11_no_M6)

(X range:    3.5250 ...   97.0250  mm)
(Y range:    3.9710 ...   44.7710  mm)

(Spindle Speed: 1000 RPM)
G21
G90
G17
G94


G01 F30.00

M5             
G00 Z7.0000
G00 X0.0000 Y0.0000                
T3
(MSG, Change to Tool Dia = 1.0000 ||| Total drills for tool T3 = 10)
M0
G00 Z7.0000
        
G01 F30.00
M03 S1000
G00 X4.0250 Y30.0500
G01 Z-0.6000
G01 Z0
G00 Z2.0000
G01 Z-1.2000
G01 Z0
G00 Z2.0000
G01 Z-1.8000
G01 Z0
G00 Z2.0000
G01 Z-2.4000
G01 Z0
G00 Z2.0000
G00 X4.0250 Y27.5100
G01 Z-0.6000
G01 Z0
G00 Z2.0000
G01 Z-1.2000
G01 Z0
G00 Z2.0000
G01 Z-1.8000
G01 Z0
G00 Z2.0000
G01 Z-2.4000
G01 Z0
G00 Z2.0000
G00 X4.0250 Y24.9700
G01 Z-0.6000
G01 Z0
G00 Z2.0000
G01 Z-1.2000
G01 Z0
G00 Z2.0000
G01 Z-1.8000
G01 Z0
G00 Z2.0000
G01 Z-2.4000
G01 Z0
G00 Z2.0000
G00 X4.0250 Y22.4300
G01 Z-0.6000
G01 Z0
G00 Z2.0000
G01 Z-1.2000
G01 Z0
G00 Z2.0000
G01 Z-1.8000
G01 Z0
G00 Z2.0000
G01 Z-2.4000
G01 Z0
G00 Z2.0000
G00 X4.0250 Y19.8900
G01 Z-0.6000
G01 Z0
G00 Z2.0000
G01 Z-1.2000
G01 Z0
G00 Z2.0000
G01 Z-1.8000
G01 Z0
G00 Z2.0000
G01 Z-2.4000
G01 Z0
G00 Z2.0000
G00 X96.5250 Y19.8900
G01 Z-0.6000
G01 Z0
G00 Z2.0000
G01 Z-1.2000
G01 Z0
G00 Z2.0000
G01 Z-1.8000
G01 Z0
G00 Z2.0000
G01 Z-2.4000
G01 Z0
G00 Z2.0000
G00 X96.5250 Y22.4300
G01 Z-0.6000
G01 Z0
G00 Z2.0000
G01 Z-1.2000
G01 Z0
G00 Z2.0000
G01 Z-1.8000
G01 Z0
G00 Z2.0000
G01 Z-2.4000
G01 Z0
G00 Z2.0000
G00 X96.5250 Y24.9700
G01 Z-0.6000
G01 Z0
G00 Z2.0000
G01 Z-1.2000
G01 Z0
G00 Z2.0000
G01 Z-1.8000
G01 Z0
G00 Z2.0000
G01 Z-2.4000
G01 Z0
G00 Z2.0000
G00 X96.5250 Y27.5100
G01 Z-0.6000
G01 Z0
G00 Z2.0000
G01 Z-1.2000
G01 Z0
G00 Z2.0000
G01 Z-1.8000
G01 Z0
G00 Z2.0000
G01 Z-2.4000
G01 Z0
G00 Z2.0000
G00 X96.5250 Y30.0500
G01 Z-0.6000
G01 Z0
G00 Z2.0000
G01 Z-1.2000
G01 Z0
G00 Z2.0000
G01 Z-1.8000
G01 Z0
G00 Z2.0000
G01 Z-2.4000
G01 Z0
G00 Z2.0000
M05
G00 Z7.00


