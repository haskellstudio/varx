$(document).ready(function() {
	$('body').wrap('<div class="container"></div>');

	$('.memtitle .permalink a').html('<span class="glyphicon glyphicon-link" aria-hidden="true"></span>');

	var headerTitle = $('.header .headertitle .title');

	headerTitle.replaceWith('<h1>' + headerTitle.html() + '</h1>');

	// $('.memberdecls .mdescRight').attr('colspan', 2);

	$('.memberdecls .memSeparator').html('');
});