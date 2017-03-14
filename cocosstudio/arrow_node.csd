<GameFile>
  <PropertyGroup Name="arrow_node" Type="Node" ID="53ee7c9b-8032-46b1-b77e-ca3e4e90288f" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="60" Speed="1.0000" ActivedAnimationName="updown">
        <Timeline ActionTag="-1032600284" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.6000" Y="0.6000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="2.0000" Y="2.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1032600284" Property="Alpha">
          <IntFrame FrameIndex="0" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="60" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="-347273854" Property="Position">
          <PointFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="30" X="0.0000" Y="10.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="60" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="updown" StartIndex="0" EndIndex="60">
          <RenderColor A="150" R="0" G="139" B="139" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Node" Tag="61" ctype="GameNodeObjectData">
        <Size X="0.0000" Y="0.0000" />
        <Children>
          <AbstractNodeData Name="a5_1" ActionTag="-1905620590" Tag="62" IconVisible="False" LeftMargin="-22.5000" RightMargin="-22.5000" TopMargin="-15.0000" BottomMargin="-15.0000" ctype="SpriteObjectData">
            <Size X="45.0000" Y="30.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="gameui/a5.png" Plist="role_ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="a11_2" ActionTag="-1032600284" Alpha="80" Tag="63" IconVisible="False" LeftMargin="-30.5000" RightMargin="-30.5000" TopMargin="-18.0000" BottomMargin="-18.0000" ctype="SpriteObjectData">
            <Size X="61.0000" Y="36.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.5567" ScaleY="1.5567" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="gameui/a11.png" Plist="role_ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="goal_jian_3" ActionTag="-347273854" Tag="64" IconVisible="False" LeftMargin="-42.5000" RightMargin="-42.5000" TopMargin="-94.3333" BottomMargin="6.3333" ctype="SpriteObjectData">
            <Size X="85.0000" Y="88.0000" />
            <AnchorPoint ScaleX="0.5000" />
            <Position Y="6.3333" />
            <Scale ScaleX="0.5000" ScaleY="0.5000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="gameui/goal_jian.png" Plist="role_ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>