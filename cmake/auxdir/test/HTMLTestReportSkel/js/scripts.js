/// Global variable for the summary content.
var test_results = {};


/**
 * Replace special char in a string
 *

var patt1=new RegExp(/[\(\)\[\]\{\}]/g);
function cleanString(data){

	return data.replace(patt1,"_");
}
*/

/**
 * Digest the summary JSON object, filling the test_results variable.
 * @param summary object containing the summary data (response of AJAJ request).
 */
function parseSummary(summary) {
	// Prepare the counters
	test_results.counter = {"PASS":     0,
			                "FAIL":     0,
			                "UNTESTED": 0,
			                "ERROR":    0,
			                "SKIPPED":  0,
			                "total":    summary.length};
	test_results.tests = {};
	test_results.not_passed = [];
	test_results.all_tests = [];
	for (var i in summary) {
		var test = summary[i];
		// Update the counter of the outcome.
		++test_results.counter[test.outcome];
		//var test_path = split(".", test.id);
		test_results.tests[test.id] = test;
		test_results.all_tests.push(test.id);
		if (test.outcome != "PASS" ) {
			test_results.not_passed.push(test.id);
		}
	}
}

/**
 * Set the html of the selected elements to a spinning icon (for visual feed back).
 * Note: the image has been downloaded from http://www.ajaxload.info
 */
jQuery.fn.loadingIcon = function() {
	return this.html('<img src="images/ajax-loader.gif"/>');
}

/**
 * Load the annotations file (annotations.json) and fill the annotations block of
 * the DOM.
 */
jQuery.fn.annotations = function(data) {
	// Collect the list of keys.
	// These keys are special and must appear first in the list.
	var keys = ["start_time", "end_time"];
	// Add all the other keys to the list of keys.
	for (var key in data) {
		if ($.inArray(key, keys) == -1) {
			keys.push(key);
		}
	}
	// Prepare a table for the annotations
	var tbody = $("<tbody/>");
	// Add the rows of the table
	for (var i in keys) {
		var key = keys[i];
		var value = data[key];
		if ($.isArray(value)) {
			// In case the value is an Array, let's format it as a list
			var tmp = $('<ul/>');
			for (i in value) {
				tmp.append($('<li/>').text(value[i]))
			}
			value = tmp;
		}
		tbody.append($("<tr/>")
				.attr("id",key)
				.append($('<td class="key"/>').text(key))

				//.append($('<a/>').text(key))
				.append($('<td class="value"/>').html(value)));
	}
	// Insert the code in the annotations block and enable the toggle button
	return this.html($('<table/>').append(tbody));
}

/// Display the summary table
jQuery.fn.summary = function() {
	var counter = test_results.counter;
	// Prepare a table layout (like the one produced usually by QMTest).
	var tbody = $("<tbody/>");
	// row with the total
	tbody.append($("<tr/>")
		 .append($("<td align='right'/>").text(counter.total))
		 .append($("<td colspan='3'/><td>tests total</td>")));
	var result_types = ["FAIL", "ERROR", "UNTESTED", "SKIPPED","PASS"];
	for (var i in result_types) {
		var result_type = result_types[i];
		if (counter[result_type]) {
			tbody.append($("<tr/>")
				     .append($("<td align='right'/>")
					     .text(counter[result_type]))
				     .append("<td>(</td>")
				     .append($("<td align='right'/>")
					     .text(Math.round(counter[result_type]
							      / counter.total * 100)))
				     .append("<td>%)</td>")
				     .append($("<td/>")
					     .text("tests " + result_type))
				     .addClass(result_type));
		}
	}
	return this.html($("<table/>").html(tbody));
}

/** Callback function for clicks on foldable li elements.
 * @returns {Boolean}
 */
function foldingAction() {
	var me = $(this);
    if (me.hasClass('folded')) {
 	   me.nextAll('div,ul').slideDown();
 	   me.find('img').attr('src', 'images/minus.png');
    } else {
 	   me.nextAll('div,ul').slideUp();
 	   me.find('img').attr('src', 'images/plus.png');
    }
    me.toggleClass("folded");
	me.toggleClass("unfolded");
    return false; // avoid bubbling of the event
}





/** Modify the items that have the "url" data, making them clickable
 *  and followed by a hidden block with the content of the link specified.
 */
jQuery.fn.loader = function() {
    this.each(function() { // loop over all the selected elements
      var me = $(this);
      if (me.data("url")) { // modify the element only if it does have a data "url"
	  me.addClass("foldable").prepend('<img src="images/plus.png"/>&nbsp;')
	      // wrap the "text" of the element with a clickable span that loads the url
	      .wrapInner($("<span class='clickable'/>")
			 .click(function(){ // trigger the loading on click
				 var me = $(this);
				 me.after($("<div/>").hide().load(me.parent().data("url"),
						 function() {
					 var me = $(this);
					 me.slideDown()
					   .prev()
					   .unbind("click") // replace the click handler
					   .click(foldingAction)
					   .find('img').attr('src', 'images/minus.png');
				 	}));
				 return false; // avoid bubbling of the event
			     }));
      }
    });
};

/**
 * Helper function to re-use the code for the toggle button callback
 */
jQuery.fn.toggleNextButton = function(data) {
	if (data === undefined) data = {};
	if (data.hide === undefined) data.hide = "(hide)";
	if (data.show === undefined) data.show = "(show)";
	if (data.start_visible === undefined) data.start_visible = false;
	this.click(function() {
		var me = $(this);
		me.next().toggle();
		if (me.next().is(":visible")) {
			me.text(data.hide);
		} else {
			me.text(data.show);
		}
	}).text(data.start_visible ? data.hide : data.show)
	  .next().toggle(data.start_visible);
	return this;
}

/** Make a given element toggleable using a show/hide button inserted just before
 *  it.
 */
jQuery.fn.makeToggleable = function(data) {
	this.each(function() {
		var btn = $("<span class='clickable display'/>");
		$(this).before(btn);
		btn.toggleNextButton(data);
	});
	return this;
}



/** Read the url parameter
 * @param name of the parameter to read
 */

function getURLParameter(name) {
    return decodeURI(
        (RegExp(name + '=' + '(.+?)(&|$)').exec(location.search)||[,null])[1]
    );
}
/**
* method which scroll to an element
*/
function goTo(element) {
        $('html, body').animate({
            scrollTop: element.offset().top + 'px'
        }, 'fast');
        return this; // for chaining...
}
/**
* function which open the theme. executed after the page is ready but all  "all_results" folder are not loaded.
*/
function openLink(){
	var test = getURLParameter("test");
	if (test != "null"){
		var theme = test.split("/")[0];

		if( theme == "annotations"){
			var me = $("#annotations_link + span");
			me.click()

		}
		else if(theme=="all_results"){

			var me = $("#"+theme).find(".clickable");

			me.click()

		}else if (theme == "results"){

		}
		else {
			alert( "Error : the attributes of the link are wrong" );
		}

	}
}
/** function to avoid useless execution of openlink2 script
 *
 * @param intoResults : true if launch by the html, false if launch after loading of all_results
 */
function openlink2Caller(intoResults){
	var test = getURLParameter("test");
	if (test != "null"){
		var adress = test.split("//")[0].split("/");
		if (adress[0] == "results" && intoResults){
			openlink2();
		}else if(adress[0] == "all_results" && !intoResults){
			openlink2();
		}
	}
}
/**
* Open the packages and the corresponding test. Highlight it too.
*/
function openlink2(){
	var test = getURLParameter("test");
	if (test != "null"){
		var adress = test.split("//")[0].split("/");

		var testName = test.split("//")[1];
		// recall the parent to open only the child avoid
		var parentPackage = undefined;

		// open the packages
		if (adress.length >=2){
			for(i=0;i<=adress.length-1;i++){
				if (parentPackage == undefined ){
					var packageTested = $("#"+adress[i]+" >span ");
					parentPackage = $("#"+adress[i]);
				}else{
					var packageTested = parentPackage.find("#"+adress[i]+" >span ")
					parentPackage = parentPackage.find("#"+adress[i])
				}
				if (packageTested != undefined){

					packageTested.click();

				}
			}
			// process the test
			var testToOpen = parentPackage.find("#"+testName);//.parent().parent();
			if (testToOpen != undefined){
				//if (testToOpen.length ==2){ testToOpen=testToOpen[1]; }
				testToOpen.click();
				goTo(testToOpen);
				testToOpen.parent().parent().addClass("highlighted");


			}
		}

	}
}

/// Code executed when the page is ready.
$(function () {
	$(".loading").loadingIcon().removeClass("loading");
	var ignore_fields = ["qmtest.cause", "qmtest.target", "causes"];
	var fields_order = ["start_time", "end_time"];

	// Add details about the project (if available)
	$.get("manifest.xml").done(function(manifest) {
		var proj = $("manifest > project", manifest);
		if (proj) {
			var text = ("for " + proj.attr("name") + " " +
					     proj.attr("version"));
			var binary_tag = $("binary_tag", manifest);
			if (binary_tag)
				text += " on " + binary_tag.text();
			$("#project-info").text(text);
			$(document).attr('title', 'Test Report ' + text);
		}
	});

	// Add details about the slot (if available)
	$.get("../../../slot-config.json").done(function(slot_config) {
		$('#slot-info').text('(slot ' + slot_config.slot + ' ' +
				slot_config.date + ')');
	});

	// load the summary
	$.get("summary.json", parseSummary, 'json')
	.done(function(){
		$("#summary").summary();
		//var ul = $("<ul />");

		// Load Test that did not pass pre-computed file
		$("#results").load("results",function(){
		    //alert($(this).html());
			$(this).find(".group.foldable > .clickable").click(foldingAction);
			$(this).find(".test.foldable > .clickable").click(foldingAction);
			$.each($(this).find(".field"),function(i,field){
				$(this).data("url",$(this).parent().attr("testName")+"/"+$(this).text());
			});
			$(this).find("li.field").loader();
			$(this).after($('<div class="clickable display">Collapse all</div>').click(function(){
				$(this).prev().find(".unfolded")
				.click();
			}));
			$(this).find(".field > span").addClass("unfolded");
			openlink2Caller(true);
		});
		//$("#results").html(ul);

		$("#all_results").html($('<span class="clickable display">(show)</span>')
				.click(function(){
					// load all_results pre-computed file
					$("#all_results").load("all_results",function(){
						$(this).find(".group.foldable > .clickable").click(foldingAction);
						$(this).find(".test.foldable > .clickable").click(foldingAction);
						$.each($(this).find(".field"),function(i,field){
							$(this).data("url",$(this).parent().attr("testName")+"/"+$(this).text());
							//alert($(this).parent().attr("testName")+"/"+$(this).text());
						});
						$(this).find("li,field").loader();
						$(this).after($('<div class="clickable display">Collapse all</div>').click(function(){
							$(this).prev().find(".unfolded")
							.click();
						}));
						$(this).find(".field > span").addClass("unfolded");
						$(this).makeToggleable({start_visible: true});
						openlink2Caller(false);
					});

				})).removeClass("loading");
		//setTimeout("openLink();",100);
		openLink();
	});

	// load annotations
	$.getJSON('annotations.json')
	.done(function(data) {
		$('#annotations').annotations(data).makeToggleable().removeClass("loading");

	});
});







