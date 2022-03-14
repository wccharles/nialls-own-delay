import AnimatedFlexBoxExample from "./AnimatedFlexBox";
import Label from "./Label";
import ParamLabel from "./ParamLabel";
import Meter from "./Meter";
import ParameterToggleButton from "./ParameterToggleButton";
import ParameterSlider from "./ParameterSlider";

import React, { Component } from "react";

import { Canvas, Image, Text, View, Slider } from "react-juce";

function animatedDraw(ctx) {
  let now = Date.now() / 10;
  let width = now % 100;
  let red = Math.sqrt(width / 100) * 255;
  let hex = Math.floor(red).toString(16);

  ctx.fillStyle = `#${hex}ffaa`;
  ctx.fillRect(0, 0, width, 2);
}

// Example of callback for image onLoad/onError
function imageLoaded() {
  console.log("Image is loaded!");
}

function imageError(error) {
  console.log(error.name);
  console.log(error.message);
}

class App extends Component {
  constructor(props) {
    super(props);
    this._onMuteToggled = this._onMuteToggled.bind(this);

    this.state = {
      muted: false,
    };
  }

  _onMuteToggled(toggled) {
    this.setState({
      muted: toggled,
    });
  }

  render() {
    // Uncomment here to watch the animated flex box example in action
    // return (
    //   <View {...styles.container}>
    //     <AnimatedFlexBoxExample />
    //   </View>
    // );

    const muteBackgroundColor = this.state.muted
      ? "#66FDCF"
      : "hsla(162, 97%, 70%, 0)";
    const muteTextColor = this.state.muted
      ? "#17191f"
      : "hsla(162, 97%, 70%, 1)";

    const sliderFillColor = "#66FDCF";
    const sliderTrackColor = "#626262";

    const logo_url =
      "https://raw.githubusercontent.com/nick-thompson/react-juce/master/examples/GainPlugin/jsui/src/logo.png";
    //const logo_png = require('./logo.png');
    //const logo_svg = require('./logo.svg');

    return (
      <View {...styles.container}>

        <View {...styles.cell}>
          <ParameterSlider
            paramId="Gain"
            onDraw={Slider.drawRotary(sliderTrackColor, sliderFillColor)}
            mapDragGestureToValue={Slider.rotaryGestureMap}
            {...styles.knob}
          >
            <Label paramId="Gain" {...styles.label} />
            <ParamLabel paramId="Gain" {...styles.text} />
          </ParameterSlider>

        </View>

        <View {...styles.cell}>

          <ParameterSlider
            paramId="DelayDryWet"
            onDraw={Slider.drawRotary(sliderTrackColor, sliderFillColor)}
            mapDragGestureToValue={Slider.rotaryGestureMap}
            {...styles.knob}
          >
            <Label paramId="DelayDryWet" {...styles.label} />
            <ParamLabel paramId="DelayDryWet" {...styles.text} />
          </ParameterSlider>

          <ParameterSlider
            paramId="DelayTime"
            onDraw={Slider.drawRotary(sliderTrackColor, sliderFillColor)}
            mapDragGestureToValue={Slider.rotaryGestureMap}
            {...styles.knob}
          >
            <Label paramId="DelayTime" {...styles.label} />
            <ParamLabel paramId="DelayTime" {...styles.text} />
          </ParameterSlider>

          <ParameterSlider
            paramId="DelayFeedback"
            onDraw={Slider.drawRotary(sliderTrackColor, sliderFillColor)}
            mapDragGestureToValue={Slider.rotaryGestureMap}
            {...styles.knob}
          >
            <Label paramId="DelayFeedback" {...styles.label} />
            <ParamLabel paramId="DelayFeedback" {...styles.text} />
        </ParameterSlider>

        </View>

        <View {...styles.cell}>
          <ParameterSlider
            paramId="FreqShift"
            onDraw={Slider.drawLinearHorizontal(sliderTrackColor, sliderFillColor)}
            mapDragGestureToValue={Slider.linearHorizontalGestureMap}
            {...styles.faderH}
          >
            <Label paramId="FreqShift" {...styles.label} />
          </ParameterSlider>
          {/* <ParamLabel paramId="FreqShift" {...styles.text} /> */}
        </View>
      </View>
    );
  }
}

const styles = {
  container: {
    width: "100%",
    height: "100%",
    backgroundColor: "#17191f",
    justifyContent: "flex-start",
    alignContent: "flex-start",
    flexWrap: "wrap",
  },
  logo: {
    flex: 0.0,
    width: "80%",
    aspectRatio: 281.6 / 35.0,
    placement: Image.PlacementFlags.centred,
  },
  knob: {
    flex: 1.0,
    minWidth: 100.0,
    minHeight: 100.0,
    width: "80%",
    height: "80%",
    flexDirection: "column",
    margin: 6.0,
    justifyContent: "space-around",
    alignItems: "center",

  },
  fader: {
    flex: 0.0,
    width: "50%",
    justifyContent: "flex-start",
    alignContent: "flex-start",
    marginTop: 15,
    marginHorizontal: "1%",
    marginBottom: 6,
  },
  label: {
    flex: 1.0,
    justifyContent: "center",
    alignItems: "center",
    marginTop: 6.0,
    interceptClickEvents: false,
  },
  meter: {
    flex: 0.0,
    width: 100.0,
    height: 16.0,
  },
  canvas: {
    flex: 0.0,
    width: 100.0,
    height: 2,
    marginTop: 10,
  },
  mute_button: {
    justifyContent: "center",
    alignItems: "center",
    borderRadius: 5.0,
    borderWidth: 2.0,
    borderColor: "rgba(102, 253, 207, 1)",
    marginTop: 10,
    minWidth: 30.0,
    minHeight: 30.0,
    width: "20%",
    height: "17.5%",
  },
  cell: {
    flex: 1.0,
    justifyContent: "center",
    backgroundColor: "#87898f",
    alignItems: "flex-start",
    margin: 6.0,
    padding: 6.0,
  },
  mute_button_text: {
    fontSize: 20.0,
    lineSpacing: 1.6,
    fontStyle: Text.FontStyleFlags.bold,
  },
  text: {
    flex: 1.0,
    fontSize: 16.0,
    lineSpacing: 1.6,
    marginTop: 6.0,
    justifyContent: "flex-end",
    // justification: Text.JustificationFlags.centredBottom,
  },
};

export default App;
