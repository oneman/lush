/*$(document).ready(function() {
  rack_units.push({'constructor': Spectrum_unit, 'id': 'spectrum_unit'});
});*/

function Spectrum_unit(info_object) {
  this.inherit_from = KRRackUnit;
  this.inherit_from();
  this.title = "Spectrum_unit";
  this.description = "For testing spectrum.";
  this.id = 'spectrum_unit'
  this.div_text = "<div class='RCU spectrum_unit' id='" + this.id + "'></div>";
  this.sel = "#" + this.id;
  info_object['parent_div'].append(this.div_text);
  $(this.sel).append('\
   <div class="spectrum_unit danred offwhite robo_light">\
    <div class="spectrum_container" id="spectrum_container"></div>\
   </div>\
  </div>');
  this.spectrum_container_elem = document.getElementById("spectrum_container");
  this.spectrum_canvas_container =
   new CanvasViewContainer(this, this.spectrum_container_elem, 'spectrum');
  this.spectrum_model = new KRSpectrumWebAudioModel(64);
  this.spectrum_view = new KRSpectrumView(this.spectrum_canvas_container,
   this.spectrum_model, this.spectrum_canvas_container.context,
   new KRBoundBox(0, 0, 1, 1),
   CanvasViewContainer.colors_from_color(MIXER_BG_COLOR), {'shadow_length': 12});
  this.spectrum_canvas_container.add_view('spectrum_unit_spectrum_view', this.spectrum_view);
}

Spectrum_unit.prototype = Object.create(KRRackUnit.prototype);
Spectrum_unit.prototype.constructor = Spectrum_unit;
