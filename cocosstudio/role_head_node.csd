<GameFile>
  <PropertyGroup Name="role_head_node" Type="Node" ID="96d974da-f946-4d86-bce1-e6fee327f090" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000" ActivedAnimationName="blink">
        <Timeline ActionTag="1526318630" Property="VisibleForFrame">
          <BoolFrame FrameIndex="0" Tween="False" Value="True" />
          <BoolFrame FrameIndex="85" Tween="False" Value="False" />
          <BoolFrame FrameIndex="90" Tween="False" Value="True" />
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="blink" StartIndex="0" EndIndex="90">
          <RenderColor A="150" R="255" G="160" B="122" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Node" Tag="33" ctype="GameNodeObjectData">
        <Size X="0.0000" Y="0.0000" />
        <Children>
          <AbstractNodeData Name="tou_eye1_2" ActionTag="830598394" Tag="35" IconVisible="False" LeftMargin="-63.5000" RightMargin="-63.5000" TopMargin="-62.0000" BottomMargin="-62.0000" ctype="SpriteObjectData">
            <Size X="127.0000" Y="124.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="gameui/tou_eye1.png" Plist="role_ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="tou_eye2_3" ActionTag="1526318630" Tag="36" IconVisible="False" LeftMargin="-63.5000" RightMargin="-63.5000" TopMargin="-62.0000" BottomMargin="-62.0000" ctype="SpriteObjectData">
            <Size X="127.0000" Y="124.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="gameui/tou_eye2.png" Plist="role_ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="tou_base_1" ActionTag="980661375" Tag="34" IconVisible="False" LeftMargin="-63.5000" RightMargin="-63.5000" TopMargin="-62.0000" BottomMargin="-62.0000" ctype="SpriteObjectData">
            <Size X="127.0000" Y="124.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="gameui/tou_base.png" Plist="role_ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>