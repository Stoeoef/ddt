function getPropertyArray(selectionModel, propName) {
	var result = [];
	for (var i = 0; i < selectionModel.rowCount; i++)
		result.push(selectionModel.getRow(i)[propName]);
	return result;
}

function setMultiPropertyByName(selectionModel, propNames, value) {
	for (var j = 0; j < propNames.length; j++) {
		for (var i = 0; i < selectionModel.rowCount; i++) {
			selectionModel.getRow(i)[propNames[j]] = value;
		}
	}
}

function setMultiPropertyById(mixedSelection, value, exclusive) {
	for (var j = 3; j < arguments.length; j++) {
		mixedSelection.setMultiProperty(arguments[j], value);
	}
	if (exclusive) {
		// set all other properties to the contrary value
		var allProps = [MixedValueSpeakerModel.OPD_FFR_ID, MixedValueSpeakerModel.OPD_GOV_ID,
						MixedValueSpeakerModel.OPD_OP_ID, MixedValueSpeakerModel.BPS_GOV_ID,
						MixedValueSpeakerModel.BPS_OP_ID, MixedValueSpeakerModel.OPD_JUD_ID,
						MixedValueSpeakerModel.BPS_JUD_ID];
		outer: for (j = 0; j < allProps.length; j++) {
			for (var i = 3; i < arguments.length; i++)
				if (arguments[i] === allProps[j])
					continue outer;
			mixedSelection.setMultiProperty(allProps[j], !value);
		}
	}
}

function toState(propertyState) {
	switch (propertyState) {
	case MixedValueSpeakerModel.ON:
		return "on";
	case MixedValueSpeakerModel.OFF:
		return "off";
	case MixedValueSpeakerModel.MIXED:
		return "mixed"
	default:
		return ""
	}
}

function getMixedState() {
	var mixed;
	var first = arguments[0];
	for (var i = 1; i < arguments.length; i++) {
		if (first !== arguments[i])
			return "mixed";
	}
	return toState(first);
}
