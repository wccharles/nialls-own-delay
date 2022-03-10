import React, { Component } from "react";
import { EventBridge, Text, View } from "react-juce";

class Label extends Component {
  constructor(props) {
    super(props);

    this._onParameterValueChange = this._onParameterValueChange.bind(this);

    this.state = {
      paramlabel: "",
    };
  }

  componentDidMount() {
    EventBridge.addListener(
      "parameterValueChange",
      this._onParameterValueChange
    );
  }

  componentWillUnmount() {
    EventBridge.removeListener(
      "parameterValueChange",
      this._onParameterValueChange
    );
  }

  _onParameterValueChange(
    index,
    paramId,
    defaultValue,
    currentValue,
    stringValue
  ) {
    if (paramId === this.props.paramId) {
      this.setState({
        paramlabel: paramId,
      });
    }
  }

  render() {
    return (
      <View {...this.props}>
        <Text {...styles.labelText}>{this.state.paramlabel}</Text>
      </View>
    );
  }
}

const styles = {
  labelText: {
    color: "#626262",
    fontSize: 16.0,
    lineSpacing: 1.6,
  },
};

export default Label;
