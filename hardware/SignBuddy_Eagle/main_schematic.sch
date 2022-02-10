<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="16" fill="1" visible="no" active="no"/>
<layer number="3" name="Route3" color="17" fill="1" visible="no" active="no"/>
<layer number="4" name="Route4" color="18" fill="1" visible="no" active="no"/>
<layer number="5" name="Route5" color="19" fill="1" visible="no" active="no"/>
<layer number="6" name="Route6" color="25" fill="1" visible="no" active="no"/>
<layer number="7" name="Route7" color="26" fill="1" visible="no" active="no"/>
<layer number="8" name="Route8" color="27" fill="1" visible="no" active="no"/>
<layer number="9" name="Route9" color="28" fill="1" visible="no" active="no"/>
<layer number="10" name="Route10" color="29" fill="1" visible="no" active="no"/>
<layer number="11" name="Route11" color="30" fill="1" visible="no" active="no"/>
<layer number="12" name="Route12" color="20" fill="1" visible="no" active="no"/>
<layer number="13" name="Route13" color="21" fill="1" visible="no" active="no"/>
<layer number="14" name="Route14" color="22" fill="1" visible="no" active="no"/>
<layer number="15" name="Route15" color="23" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="24" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="stm32l053">
<packages>
<package name="LQFP48-7X7MM">
<smd name="1" x="-4.25" y="2.75" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="2" x="-4.25" y="2.25" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="3" x="-4.25" y="1.75" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="4" x="-4.25" y="1.25" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="5" x="-4.25" y="0.75" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="6" x="-4.25" y="0.25" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="7" x="-4.25" y="-0.25" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="8" x="-4.25" y="-0.75" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="9" x="-4.25" y="-1.25" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="10" x="-4.25" y="-1.75" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="11" x="-4.25" y="-2.25" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="12" x="-4.25" y="-2.75" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="13" x="-2.75" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="14" x="-2.25" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="15" x="-1.75" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="16" x="-1.25" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="17" x="-0.75" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="18" x="-0.25" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="19" x="0.25" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="20" x="0.75" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="21" x="1.25" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="22" x="1.75" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="23" x="2.25" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="24" x="2.75" y="-4.25" dx="1.1938" dy="0.3048" layer="1" rot="R90"/>
<smd name="25" x="4.25" y="-2.75" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="26" x="4.25" y="-2.25" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="27" x="4.25" y="-1.75" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="28" x="4.25" y="-1.25" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="29" x="4.25" y="-0.75" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="30" x="4.25" y="-0.25" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="31" x="4.25" y="0.25" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="32" x="4.25" y="0.75" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="33" x="4.25" y="1.25" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="34" x="4.25" y="1.75" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="35" x="4.25" y="2.25" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="36" x="4.25" y="2.75" dx="1.1938" dy="0.3048" layer="1" rot="R180"/>
<smd name="37" x="2.75" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<smd name="38" x="2.25" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<smd name="39" x="1.75" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<smd name="40" x="1.25" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<smd name="41" x="0.75" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<smd name="42" x="0.25" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<smd name="43" x="-0.25" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<smd name="44" x="-0.75" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<smd name="45" x="-1.25" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<smd name="46" x="-1.75" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<smd name="47" x="-2.25" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<smd name="48" x="-2.75" y="4.25" dx="1.1938" dy="0.3048" layer="1" rot="R270"/>
<wire x1="-3.7084" y1="-3.7084" x2="-3.2258" y2="-3.7084" width="0.1524" layer="21"/>
<wire x1="3.7084" y1="-3.7084" x2="3.7084" y2="-3.2258" width="0.1524" layer="21"/>
<wire x1="3.7084" y1="3.7084" x2="3.2258" y2="3.7084" width="0.1524" layer="21"/>
<wire x1="-3.7084" y1="3.7084" x2="-3.7084" y2="3.2258" width="0.1524" layer="21"/>
<wire x1="-3.7084" y1="-3.2258" x2="-3.7084" y2="-3.7084" width="0.1524" layer="21"/>
<wire x1="3.2258" y1="-3.7084" x2="3.7084" y2="-3.7084" width="0.1524" layer="21"/>
<wire x1="3.7084" y1="3.2258" x2="3.7084" y2="3.7084" width="0.1524" layer="21"/>
<wire x1="-3.2258" y1="3.7084" x2="-3.7084" y2="3.7084" width="0.1524" layer="21"/>
<wire x1="-5.2832" y1="2.7432" x2="-5.4356" y2="2.7432" width="0.1524" layer="21" curve="-180"/>
<wire x1="-5.4356" y1="2.7432" x2="-5.2832" y2="2.7432" width="0.1524" layer="21" curve="-180"/>
<polygon width="0.0254" layer="21">
<vertex x="-5.3549" y="-1.5595"/>
<vertex x="-5.3549" y="-1.9405"/>
<vertex x="-5.1009" y="-1.9405"/>
<vertex x="-5.1009" y="-1.5595"/>
</polygon>
<polygon width="0.0254" layer="21">
<vertex x="0.5595" y="-5.1009"/>
<vertex x="0.5595" y="-5.3549"/>
<vertex x="0.9405" y="-5.3549"/>
<vertex x="0.9405" y="-5.1009"/>
</polygon>
<polygon width="0.0254" layer="21">
<vertex x="5.3549" y="-0.0595"/>
<vertex x="5.3549" y="-0.4405"/>
<vertex x="5.1009" y="-0.4405"/>
<vertex x="5.1009" y="-0.0595"/>
</polygon>
<polygon width="0.0254" layer="21">
<vertex x="1.0595" y="5.1009"/>
<vertex x="1.0595" y="5.3549"/>
<vertex x="1.4405" y="5.3549"/>
<vertex x="1.4405" y="5.1009"/>
</polygon>
<text x="-1.7272" y="-0.635" size="1.27" layer="21" ratio="6" rot="SR0">&gt;Value</text>
<polygon width="0.1524" layer="41">
<vertex x="-5.4819" y="-5.4819"/>
<vertex x="-5.4819" y="5.4819"/>
<vertex x="5.4819" y="5.4819"/>
<vertex x="5.4819" y="-5.4819"/>
</polygon>
<wire x1="-4.2418" y1="2.7432" x2="-5.9944" y2="2.7432" width="0.1524" layer="47"/>
<wire x1="-5.9944" y1="2.7432" x2="-6.3754" y2="2.7432" width="0.1524" layer="47"/>
<wire x1="-4.2418" y1="2.2606" x2="-5.9944" y2="2.2606" width="0.1524" layer="47"/>
<wire x1="-5.9944" y1="2.2606" x2="-6.3754" y2="2.2606" width="0.1524" layer="47"/>
<wire x1="-5.9944" y1="2.7432" x2="-5.9944" y2="4.0132" width="0.1524" layer="47"/>
<wire x1="-5.9944" y1="2.2606" x2="-5.9944" y2="0.9906" width="0.1524" layer="47"/>
<wire x1="-5.9944" y1="2.7432" x2="-6.1214" y2="2.9972" width="0.1524" layer="47"/>
<wire x1="-5.9944" y1="2.7432" x2="-5.8674" y2="2.9972" width="0.1524" layer="47"/>
<wire x1="-6.1214" y1="2.9972" x2="-5.8674" y2="2.9972" width="0.1524" layer="47"/>
<wire x1="-5.9944" y1="2.2606" x2="-6.1214" y2="2.0066" width="0.1524" layer="47"/>
<wire x1="-5.9944" y1="2.2606" x2="-5.8674" y2="2.0066" width="0.1524" layer="47"/>
<wire x1="-6.1214" y1="2.0066" x2="-5.8674" y2="2.0066" width="0.1524" layer="47"/>
<wire x1="3.048" y1="4.2418" x2="5.9944" y2="4.2418" width="0.1524" layer="47"/>
<wire x1="5.9944" y1="4.2418" x2="6.3754" y2="4.2418" width="0.1524" layer="47"/>
<wire x1="3.048" y1="-4.2418" x2="5.9944" y2="-4.2418" width="0.1524" layer="47"/>
<wire x1="5.9944" y1="-4.2418" x2="6.3754" y2="-4.2418" width="0.1524" layer="47"/>
<wire x1="5.9944" y1="4.2418" x2="5.9944" y2="-4.2418" width="0.1524" layer="47"/>
<wire x1="5.9944" y1="4.2418" x2="5.8674" y2="3.9878" width="0.1524" layer="47"/>
<wire x1="5.9944" y1="4.2418" x2="6.1214" y2="3.9878" width="0.1524" layer="47"/>
<wire x1="5.8674" y1="3.9878" x2="6.1214" y2="3.9878" width="0.1524" layer="47"/>
<wire x1="5.9944" y1="-4.2418" x2="5.8674" y2="-3.9878" width="0.1524" layer="47"/>
<wire x1="5.9944" y1="-4.2418" x2="6.1214" y2="-3.9878" width="0.1524" layer="47"/>
<wire x1="5.8674" y1="-3.9878" x2="6.1214" y2="-3.9878" width="0.1524" layer="47"/>
<wire x1="-4.2418" y1="-2.4384" x2="-4.2418" y2="-6.3754" width="0.1524" layer="47"/>
<wire x1="-4.2418" y1="-6.3754" x2="-4.2418" y2="-6.7564" width="0.1524" layer="47"/>
<wire x1="4.2418" y1="-2.4384" x2="4.2418" y2="-6.3754" width="0.1524" layer="47"/>
<wire x1="4.2418" y1="-6.3754" x2="4.2418" y2="-6.7564" width="0.1524" layer="47"/>
<wire x1="-4.2418" y1="-6.3754" x2="4.2418" y2="-6.3754" width="0.1524" layer="47"/>
<wire x1="-4.2418" y1="-6.3754" x2="-3.9878" y2="-6.2484" width="0.1524" layer="47"/>
<wire x1="-4.2418" y1="-6.3754" x2="-3.9878" y2="-6.5024" width="0.1524" layer="47"/>
<wire x1="-3.9878" y1="-6.2484" x2="-3.9878" y2="-6.5024" width="0.1524" layer="47"/>
<wire x1="4.2418" y1="-6.3754" x2="3.9878" y2="-6.2484" width="0.1524" layer="47"/>
<wire x1="4.2418" y1="-6.3754" x2="3.9878" y2="-6.5024" width="0.1524" layer="47"/>
<wire x1="3.9878" y1="-6.2484" x2="3.9878" y2="-6.5024" width="0.1524" layer="47"/>
<wire x1="-3.6068" y1="-2.7432" x2="-3.6068" y2="-6.9342" width="0.1524" layer="47"/>
<wire x1="-3.6068" y1="-6.9342" x2="-3.6068" y2="-7.3152" width="0.1524" layer="47"/>
<wire x1="3.6068" y1="-2.7432" x2="3.6068" y2="-6.9342" width="0.1524" layer="47"/>
<wire x1="3.6068" y1="-6.9342" x2="3.6068" y2="-7.3152" width="0.1524" layer="47"/>
<wire x1="-3.6068" y1="-6.9342" x2="3.6068" y2="-6.9342" width="0.1524" layer="47"/>
<wire x1="-3.6068" y1="-6.9342" x2="-3.3528" y2="-6.8072" width="0.1524" layer="47"/>
<wire x1="-3.6068" y1="-6.9342" x2="-3.3528" y2="-7.0612" width="0.1524" layer="47"/>
<wire x1="-3.3528" y1="-6.8072" x2="-3.3528" y2="-7.0612" width="0.1524" layer="47"/>
<wire x1="3.6068" y1="-6.9342" x2="3.3528" y2="-6.8072" width="0.1524" layer="47"/>
<wire x1="3.6068" y1="-6.9342" x2="3.3528" y2="-7.0612" width="0.1524" layer="47"/>
<wire x1="3.3528" y1="-6.8072" x2="3.3528" y2="-7.0612" width="0.1524" layer="47"/>
<wire x1="-2.7432" y1="3.6068" x2="-6.9342" y2="3.6068" width="0.1524" layer="47"/>
<wire x1="-6.9342" y1="3.6068" x2="-7.3152" y2="3.6068" width="0.1524" layer="47"/>
<wire x1="-2.7432" y1="-3.6068" x2="-6.9342" y2="-3.6068" width="0.1524" layer="47"/>
<wire x1="-6.9342" y1="-3.6068" x2="-7.3152" y2="-3.6068" width="0.1524" layer="47"/>
<wire x1="-6.9342" y1="3.6068" x2="-6.9342" y2="-3.6068" width="0.1524" layer="47"/>
<wire x1="-6.9342" y1="3.6068" x2="-7.0612" y2="3.3528" width="0.1524" layer="47"/>
<wire x1="-6.9342" y1="3.6068" x2="-6.8072" y2="3.3528" width="0.1524" layer="47"/>
<wire x1="-7.0612" y1="3.3528" x2="-6.8072" y2="3.3528" width="0.1524" layer="47"/>
<wire x1="-6.9342" y1="-3.6068" x2="-7.0612" y2="-3.3528" width="0.1524" layer="47"/>
<wire x1="-6.9342" y1="-3.6068" x2="-6.8072" y2="-3.3528" width="0.1524" layer="47"/>
<wire x1="-7.0612" y1="-3.3528" x2="-6.8072" y2="-3.3528" width="0.1524" layer="47"/>
<text x="-15.2146" y="-11.4554" size="1.27" layer="47" ratio="6" rot="SR0">Default Padstyle: RX47Y12D0T</text>
<text x="-14.8082" y="-16.0274" size="1.27" layer="47" ratio="6" rot="SR0">Alt 1 Padstyle: OX60Y90D30P</text>
<text x="-14.8082" y="-17.5514" size="1.27" layer="47" ratio="6" rot="SR0">Alt 2 Padstyle: OX90Y60D30P</text>
<text x="-12.8524" y="2.1844" size="0.635" layer="47" ratio="4" rot="SR0">0.02in/0.5mm</text>
<text x="6.5024" y="-0.3048" size="0.635" layer="47" ratio="4" rot="SR0">0.335in/8.5mm</text>
<text x="-3.4544" y="-7.5184" size="0.635" layer="47" ratio="4" rot="SR0">0.335in/8.5mm</text>
<text x="-4.0386" y="-8.0772" size="0.635" layer="47" ratio="4" rot="SR0">0.283in/7.188mm</text>
<text x="-15.5448" y="-0.3048" size="0.635" layer="47" ratio="4" rot="SR0">0.283in/7.188mm</text>
<wire x1="-3.6068" y1="2.3368" x2="-2.3368" y2="3.6068" width="0.1524" layer="51"/>
<wire x1="-3.6068" y1="-3.6068" x2="3.6068" y2="-3.6068" width="0.1524" layer="51"/>
<wire x1="3.6068" y1="-3.6068" x2="3.6068" y2="3.6068" width="0.1524" layer="51"/>
<wire x1="3.6068" y1="3.6068" x2="-3.6068" y2="3.6068" width="0.1524" layer="51"/>
<wire x1="-3.6068" y1="3.6068" x2="-3.6068" y2="-3.6068" width="0.1524" layer="51"/>
<wire x1="-2.9464" y1="2.7432" x2="-3.0988" y2="2.7432" width="0" layer="51" curve="-180"/>
<wire x1="-3.0988" y1="2.7432" x2="-2.9464" y2="2.7432" width="0" layer="51" curve="-180"/>
<text x="-3.2766" y="-0.635" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Name</text>
</package>
</packages>
<symbols>
<symbol name="STM32L0X3CL4_1">
<pin name="PA0" x="2.54" y="0" length="middle"/>
<pin name="PA1" x="2.54" y="-2.54" length="middle"/>
<pin name="PA2" x="2.54" y="-5.08" length="middle"/>
<pin name="PA3" x="2.54" y="-7.62" length="middle"/>
<pin name="PA4" x="2.54" y="-10.16" length="middle"/>
<pin name="PA5" x="2.54" y="-12.7" length="middle"/>
<pin name="PA6" x="2.54" y="-15.24" length="middle"/>
<pin name="PA7" x="2.54" y="-17.78" length="middle"/>
<pin name="PA8" x="2.54" y="-20.32" length="middle"/>
<pin name="PA9" x="2.54" y="-22.86" length="middle"/>
<pin name="PA10" x="2.54" y="-25.4" length="middle"/>
<pin name="PA11" x="2.54" y="-27.94" length="middle"/>
<pin name="PA12" x="2.54" y="-30.48" length="middle"/>
<pin name="PA13" x="2.54" y="-33.02" length="middle"/>
<pin name="PA14" x="2.54" y="-35.56" length="middle"/>
<pin name="PA15" x="2.54" y="-38.1" length="middle"/>
<pin name="PB0" x="2.54" y="-43.18" length="middle"/>
<pin name="PB1" x="2.54" y="-45.72" length="middle"/>
<pin name="PB2" x="2.54" y="-48.26" length="middle"/>
<pin name="PB3" x="2.54" y="-50.8" length="middle"/>
<pin name="PB4" x="2.54" y="-53.34" length="middle"/>
<pin name="PB5" x="2.54" y="-55.88" length="middle"/>
<pin name="PB6" x="2.54" y="-58.42" length="middle"/>
<pin name="PB7" x="2.54" y="-60.96" length="middle"/>
<pin name="PB8" x="2.54" y="-63.5" length="middle"/>
<pin name="PB9" x="2.54" y="-66.04" length="middle"/>
<pin name="PB10" x="2.54" y="-68.58" length="middle"/>
<pin name="PB11" x="2.54" y="-71.12" length="middle"/>
<pin name="PB12" x="2.54" y="-73.66" length="middle"/>
<pin name="PB13" x="2.54" y="-76.2" length="middle"/>
<pin name="PB14" x="2.54" y="-78.74" length="middle"/>
<pin name="PB15" x="2.54" y="-81.28" length="middle"/>
<pin name="PC13" x="63.5" y="0" length="middle" rot="R180"/>
<pin name="PC14-OSC32_IN" x="63.5" y="-2.54" length="middle" rot="R180"/>
<pin name="PC15-OSC32_OUT" x="63.5" y="-5.08" length="middle" rot="R180"/>
<pin name="PH0-OSC_IN" x="63.5" y="-10.16" length="middle" rot="R180"/>
<pin name="PH1-OSC_OUT" x="63.5" y="-12.7" length="middle" rot="R180"/>
<pin name="BOOT0" x="63.5" y="-17.78" length="middle" direction="in" rot="R180"/>
<pin name="NRST" x="63.5" y="-22.86" length="middle" rot="R180"/>
<wire x1="7.112" y1="0" x2="6.0452" y2="0.508" width="0.1524" layer="94"/>
<wire x1="7.112" y1="0" x2="6.0452" y2="-0.508" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="0.508" x2="4.4958" y2="0" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-0.508" x2="4.4958" y2="0" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-2.54" x2="6.0452" y2="-2.032" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-2.54" x2="6.0452" y2="-3.048" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-2.032" x2="4.4958" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-3.048" x2="4.4958" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-5.08" x2="6.0452" y2="-4.572" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-5.08" x2="6.0452" y2="-5.588" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-4.572" x2="4.4958" y2="-5.08" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-5.588" x2="4.4958" y2="-5.08" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-7.62" x2="6.0452" y2="-7.112" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-7.62" x2="6.0452" y2="-8.128" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-7.112" x2="4.4958" y2="-7.62" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-8.128" x2="4.4958" y2="-7.62" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-10.16" x2="6.0452" y2="-9.652" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-10.16" x2="6.0452" y2="-10.668" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-9.652" x2="4.4958" y2="-10.16" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-10.668" x2="4.4958" y2="-10.16" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-12.7" x2="6.0452" y2="-12.192" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-12.7" x2="6.0452" y2="-13.208" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-12.192" x2="4.4958" y2="-12.7" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-13.208" x2="4.4958" y2="-12.7" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-15.24" x2="6.0452" y2="-14.732" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-15.24" x2="6.0452" y2="-15.748" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-14.732" x2="4.4958" y2="-15.24" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-15.748" x2="4.4958" y2="-15.24" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-17.78" x2="6.0452" y2="-17.272" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-17.78" x2="6.0452" y2="-18.288" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-17.272" x2="4.4958" y2="-17.78" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-18.288" x2="4.4958" y2="-17.78" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-20.32" x2="6.0452" y2="-19.812" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-20.32" x2="6.0452" y2="-20.828" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-19.812" x2="4.4958" y2="-20.32" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-20.828" x2="4.4958" y2="-20.32" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-22.86" x2="6.0452" y2="-22.352" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-22.86" x2="6.0452" y2="-23.368" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-22.352" x2="4.4958" y2="-22.86" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-23.368" x2="4.4958" y2="-22.86" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-25.4" x2="6.0452" y2="-24.892" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-25.4" x2="6.0452" y2="-25.908" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-24.892" x2="4.4958" y2="-25.4" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-25.908" x2="4.4958" y2="-25.4" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-27.94" x2="6.0452" y2="-27.432" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-27.94" x2="6.0452" y2="-28.448" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-27.432" x2="4.4958" y2="-27.94" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-28.448" x2="4.4958" y2="-27.94" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-30.48" x2="6.0452" y2="-29.972" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-30.48" x2="6.0452" y2="-30.988" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-29.972" x2="4.4958" y2="-30.48" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-30.988" x2="4.4958" y2="-30.48" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-33.02" x2="6.0452" y2="-32.512" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-33.02" x2="6.0452" y2="-33.528" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-32.512" x2="4.4958" y2="-33.02" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-33.528" x2="4.4958" y2="-33.02" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-35.56" x2="6.0452" y2="-35.052" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-35.56" x2="6.0452" y2="-36.068" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-35.052" x2="4.4958" y2="-35.56" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-36.068" x2="4.4958" y2="-35.56" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-38.1" x2="6.0452" y2="-37.592" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-38.1" x2="6.0452" y2="-38.608" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-37.592" x2="4.4958" y2="-38.1" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-38.608" x2="4.4958" y2="-38.1" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-43.18" x2="6.0452" y2="-42.672" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-43.18" x2="6.0452" y2="-43.688" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-42.672" x2="4.4958" y2="-43.18" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-43.688" x2="4.4958" y2="-43.18" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-45.72" x2="6.0452" y2="-45.212" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-45.72" x2="6.0452" y2="-46.228" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-45.212" x2="4.4958" y2="-45.72" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-46.228" x2="4.4958" y2="-45.72" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-48.26" x2="6.0452" y2="-47.752" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-48.26" x2="6.0452" y2="-48.768" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-47.752" x2="4.4958" y2="-48.26" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-48.768" x2="4.4958" y2="-48.26" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-50.8" x2="6.0452" y2="-50.292" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-50.8" x2="6.0452" y2="-51.308" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-50.292" x2="4.4958" y2="-50.8" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-51.308" x2="4.4958" y2="-50.8" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-53.34" x2="6.0452" y2="-52.832" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-53.34" x2="6.0452" y2="-53.848" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-52.832" x2="4.4958" y2="-53.34" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-53.848" x2="4.4958" y2="-53.34" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-55.88" x2="6.0452" y2="-55.372" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-55.88" x2="6.0452" y2="-56.388" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-55.372" x2="4.4958" y2="-55.88" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-56.388" x2="4.4958" y2="-55.88" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-58.42" x2="6.0452" y2="-57.912" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-58.42" x2="6.0452" y2="-58.928" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-57.912" x2="4.4958" y2="-58.42" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-58.928" x2="4.4958" y2="-58.42" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-60.96" x2="6.0452" y2="-60.452" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-60.96" x2="6.0452" y2="-61.468" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-60.452" x2="4.4958" y2="-60.96" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-61.468" x2="4.4958" y2="-60.96" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-63.5" x2="6.0452" y2="-62.992" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-63.5" x2="6.0452" y2="-64.008" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-62.992" x2="4.4958" y2="-63.5" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-64.008" x2="4.4958" y2="-63.5" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-66.04" x2="6.0452" y2="-65.532" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-66.04" x2="6.0452" y2="-66.548" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-65.532" x2="4.4958" y2="-66.04" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-66.548" x2="4.4958" y2="-66.04" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-68.58" x2="6.0452" y2="-68.072" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-68.58" x2="6.0452" y2="-69.088" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-68.072" x2="4.4958" y2="-68.58" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-69.088" x2="4.4958" y2="-68.58" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-71.12" x2="6.0452" y2="-70.612" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-71.12" x2="6.0452" y2="-71.628" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-70.612" x2="4.4958" y2="-71.12" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-71.628" x2="4.4958" y2="-71.12" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-73.66" x2="6.0452" y2="-73.152" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-73.66" x2="6.0452" y2="-74.168" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-73.152" x2="4.4958" y2="-73.66" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-74.168" x2="4.4958" y2="-73.66" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-76.2" x2="6.0452" y2="-75.692" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-76.2" x2="6.0452" y2="-76.708" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-75.692" x2="4.4958" y2="-76.2" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-76.708" x2="4.4958" y2="-76.2" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-78.74" x2="6.0452" y2="-78.232" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-78.74" x2="6.0452" y2="-79.248" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-78.232" x2="4.4958" y2="-78.74" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-79.248" x2="4.4958" y2="-78.74" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-81.28" x2="6.0452" y2="-80.772" width="0.1524" layer="94"/>
<wire x1="7.112" y1="-81.28" x2="6.0452" y2="-81.788" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-80.772" x2="4.4958" y2="-81.28" width="0.1524" layer="94"/>
<wire x1="5.5372" y1="-81.788" x2="4.4958" y2="-81.28" width="0.1524" layer="94"/>
<wire x1="58.928" y1="0" x2="59.9948" y2="0.508" width="0.1524" layer="94"/>
<wire x1="58.928" y1="0" x2="59.9948" y2="-0.508" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="0.508" x2="61.5442" y2="0" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="-0.508" x2="61.5442" y2="0" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-2.54" x2="59.9948" y2="-2.032" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-2.54" x2="59.9948" y2="-3.048" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="-2.032" x2="61.5442" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="-3.048" x2="61.5442" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-5.08" x2="59.9948" y2="-4.572" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-5.08" x2="59.9948" y2="-5.588" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="-4.572" x2="61.5442" y2="-5.08" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="-5.588" x2="61.5442" y2="-5.08" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-10.16" x2="59.9948" y2="-9.652" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-10.16" x2="59.9948" y2="-10.668" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="-9.652" x2="61.5442" y2="-10.16" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="-10.668" x2="61.5442" y2="-10.16" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-12.7" x2="59.9948" y2="-12.192" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-12.7" x2="59.9948" y2="-13.208" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="-12.192" x2="61.5442" y2="-12.7" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="-13.208" x2="61.5442" y2="-12.7" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-17.78" x2="59.9948" y2="-17.272" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-17.78" x2="59.9948" y2="-18.288" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-22.86" x2="59.9948" y2="-22.352" width="0.1524" layer="94"/>
<wire x1="58.928" y1="-22.86" x2="59.9948" y2="-23.368" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="-22.352" x2="61.5442" y2="-22.86" width="0.1524" layer="94"/>
<wire x1="60.5028" y1="-23.368" x2="61.5442" y2="-22.86" width="0.1524" layer="94"/>
<wire x1="7.62" y1="5.08" x2="7.62" y2="-86.36" width="0.1524" layer="94"/>
<wire x1="7.62" y1="-86.36" x2="58.42" y2="-86.36" width="0.1524" layer="94"/>
<wire x1="58.42" y1="-86.36" x2="58.42" y2="5.08" width="0.1524" layer="94"/>
<wire x1="58.42" y1="5.08" x2="7.62" y2="5.08" width="0.1524" layer="94"/>
<text x="28.2956" y="9.1186" size="2.0828" layer="95" ratio="6" rot="SR0">&gt;Name</text>
<text x="27.6606" y="6.5786" size="2.0828" layer="96" ratio="6" rot="SR0">&gt;Value</text>
</symbol>
<symbol name="STM32L0X3CL4">
<pin name="VSS_2" x="2.54" y="0" length="middle" direction="pwr"/>
<pin name="VSS_3" x="2.54" y="-2.54" length="middle" direction="pwr"/>
<pin name="VSS" x="2.54" y="-5.08" length="middle" direction="pwr"/>
<pin name="VSSA" x="2.54" y="-10.16" length="middle" direction="pwr"/>
<pin name="VLCD" x="2.54" y="-15.24" length="middle" direction="pwr"/>
<pin name="VDD_2" x="43.18" y="0" length="middle" direction="pwr" rot="R180"/>
<pin name="VDD" x="43.18" y="-2.54" length="middle" direction="pwr" rot="R180"/>
<pin name="VDDA" x="43.18" y="-7.62" length="middle" direction="pwr" rot="R180"/>
<pin name="VDD_USB" x="43.18" y="-12.7" length="middle" direction="pwr" rot="R180"/>
<wire x1="7.62" y1="5.08" x2="7.62" y2="-20.32" width="0.1524" layer="94"/>
<wire x1="7.62" y1="-20.32" x2="38.1" y2="-20.32" width="0.1524" layer="94"/>
<wire x1="38.1" y1="-20.32" x2="38.1" y2="5.08" width="0.1524" layer="94"/>
<wire x1="38.1" y1="5.08" x2="7.62" y2="5.08" width="0.1524" layer="94"/>
<text x="18.1356" y="9.1186" size="2.0828" layer="95" ratio="6" rot="SR0">&gt;Name</text>
<text x="17.5006" y="6.5786" size="2.0828" layer="96" ratio="6" rot="SR0">&gt;Value</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="STM32L053C8T6D" prefix="U">
<gates>
<gate name="A" symbol="STM32L0X3CL4_1" x="0" y="0"/>
<gate name="B" symbol="STM32L0X3CL4" x="79.756" y="0"/>
</gates>
<devices>
<device name="" package="LQFP48-7X7MM">
<connects>
<connect gate="A" pin="BOOT0" pad="44"/>
<connect gate="A" pin="NRST" pad="7"/>
<connect gate="A" pin="PA0" pad="10"/>
<connect gate="A" pin="PA1" pad="11"/>
<connect gate="A" pin="PA10" pad="31"/>
<connect gate="A" pin="PA11" pad="32"/>
<connect gate="A" pin="PA12" pad="33"/>
<connect gate="A" pin="PA13" pad="34"/>
<connect gate="A" pin="PA14" pad="37"/>
<connect gate="A" pin="PA15" pad="38"/>
<connect gate="A" pin="PA2" pad="12"/>
<connect gate="A" pin="PA3" pad="13"/>
<connect gate="A" pin="PA4" pad="14"/>
<connect gate="A" pin="PA5" pad="15"/>
<connect gate="A" pin="PA6" pad="16"/>
<connect gate="A" pin="PA7" pad="17"/>
<connect gate="A" pin="PA8" pad="29"/>
<connect gate="A" pin="PA9" pad="30"/>
<connect gate="A" pin="PB0" pad="18"/>
<connect gate="A" pin="PB1" pad="19"/>
<connect gate="A" pin="PB10" pad="21"/>
<connect gate="A" pin="PB11" pad="22"/>
<connect gate="A" pin="PB12" pad="25"/>
<connect gate="A" pin="PB13" pad="26"/>
<connect gate="A" pin="PB14" pad="27"/>
<connect gate="A" pin="PB15" pad="28"/>
<connect gate="A" pin="PB2" pad="20"/>
<connect gate="A" pin="PB3" pad="39"/>
<connect gate="A" pin="PB4" pad="40"/>
<connect gate="A" pin="PB5" pad="41"/>
<connect gate="A" pin="PB6" pad="42"/>
<connect gate="A" pin="PB7" pad="43"/>
<connect gate="A" pin="PB8" pad="45"/>
<connect gate="A" pin="PB9" pad="46"/>
<connect gate="A" pin="PC13" pad="2"/>
<connect gate="A" pin="PC14-OSC32_IN" pad="3"/>
<connect gate="A" pin="PC15-OSC32_OUT" pad="4"/>
<connect gate="A" pin="PH0-OSC_IN" pad="5"/>
<connect gate="A" pin="PH1-OSC_OUT" pad="6"/>
<connect gate="B" pin="VDD" pad="48"/>
<connect gate="B" pin="VDDA" pad="9"/>
<connect gate="B" pin="VDD_2" pad="36"/>
<connect gate="B" pin="VDD_USB" pad="24"/>
<connect gate="B" pin="VLCD" pad="1"/>
<connect gate="B" pin="VSS" pad="47"/>
<connect gate="B" pin="VSSA" pad="8"/>
<connect gate="B" pin="VSS_2" pad="23"/>
<connect gate="B" pin="VSS_3" pad="35"/>
</connects>
<technologies>
<technology name="">
<attribute name="COPYRIGHT" value="Copyright (C) 2022 Ultra Librarian. All rights reserved." constant="no"/>
<attribute name="DIGI-KEY_PART_NUMBER_1" value="STM32L053C8T6D-ND" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="STM32L053C8T6D" constant="no"/>
<attribute name="MFR_NAME" value="STMicroelectronics" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U1" library="stm32l053" deviceset="STM32L053C8T6D" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U1" gate="A" x="50.8" y="96.52" smashed="yes">
<attribute name="NAME" x="79.0956" y="105.6386" size="2.0828" layer="95" ratio="6" rot="SR0"/>
<attribute name="VALUE" x="78.4606" y="103.0986" size="2.0828" layer="96" ratio="6" rot="SR0"/>
</instance>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
