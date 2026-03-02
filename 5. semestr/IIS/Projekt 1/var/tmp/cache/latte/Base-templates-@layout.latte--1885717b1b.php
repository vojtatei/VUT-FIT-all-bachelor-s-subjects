<?php

use Latte\Runtime as LR;

/** source: /var/www/html/app/UI/Modules/Base/templates/@layout.latte */
final class Template1885717b1b extends Latte\Runtime\Template
{
	protected const BLOCKS = [
		0 => ['title' => 'blockTitle', 'head' => 'blockHead', 'main' => 'blockMain'],
		'snippet' => ['flashes' => 'blockFlashes'],
	];


	public function main(): array
	{
		extract($this->params);
		echo '<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
	<meta http-equiv="X-UA-Compatible" content="ie=edge">
	<link rel="shortcut icon" href="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 7 */;
		echo '/favicon.ico">

	<!-- Seo -->
	<title>';
		if ($this->getParentName()) {
			return get_defined_vars();
		}
		$this->renderBlock('title', get_defined_vars(), function ($s, $type) {
			$ʟ_fi = new LR\FilterInfo($type);
			return LR\Filters::convertTo($ʟ_fi, 'html', $this->filters->filterContent('trim', $ʟ_fi, $this->filters->filterContent('stripHtml', $ʟ_fi, $s)));
		}) /* line 10 */;
		echo '</title>

	<!-- Meta -->
';
		if ($this->hasBlock("description")) /* line 13 */ {
			echo '	<meta name="description" content="';
			$this->renderBlock('description', [], 'htmlAttr') /* line 13 */;
			echo '">
';
		}
		if ($this->hasBlock("keywords")) /* line 14 */ {
			echo '	<meta name="keywords" content="';
			$this->renderBlock('keywords', [], 'htmlAttr') /* line 14 */;
			echo '">
';
		}
		echo '	<meta name="robots" content="index,follow">
	<meta name="googlebot" content="snippet,archive">
	<meta name="author" content="f3l1x">

	<link href="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 19 */;
		echo '/css/bootstrap.min.css" rel="stylesheet">
	<link href="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 20 */;
		echo '/css/font-awesome.min.css" rel="stylesheet">
	<link rel="stylesheet" href="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 21 */;
		echo '/assets/datagrid.css">
	<link rel="stylesheet" href="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 22 */;
		echo '/assets/datagrid-spinners.css">

	';
		$this->renderBlock('head', get_defined_vars()) /* line 24 */;
		echo '


</head>
<body>
<script src="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 29 */;
		echo '/assets/jquery-3.7.1.min.js"></script>
<script src="https://unpkg.com/naja@3/dist/Naja.min.js"></script>
<script>
	naja.initialize();
	console.log("NAJA INITIALIZED");
</script>
<script src="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 35 */;
		echo '/assets/datagrid.js"></script>
<script src="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 36 */;
		echo '/assets/datagrid-spinners.js"></script>
<script src="';
		echo LR\Filters::escapeHtmlAttr(LR\Filters::safeUrl($basePath)) /* line 37 */;
		echo '/js/bootstrap.bundle.min.js"></script>
<script>
	$(document).ready(function(){
		$(\'.dropdown-toggle\').dropdown()
	});
</script>
<script src="https://unpkg.com/nette-forms@3"></script>

<div id="';
		echo htmlspecialchars($this->global->snippetDriver->getHtmlId('flashes'));
		echo '">';
		$this->renderBlock('flashes', [], null, 'snippet') /* line 45 */;
		echo '</div>
';
		$this->renderBlock('main', get_defined_vars()) /* line 48 */;
		echo '

</body>
</html>
';
		return get_defined_vars();
	}


	public function prepare(): void
	{
		extract($this->params);
		Nette\Bridges\ApplicationLatte\UIRuntime::initialize($this, $this->parentName, $this->blocks);
		
	}


	/** {block #title} on line 10 */
	public function blockTitle(array $ʟ_args): void
	{
		echo 'EDU-Konf';
	}


	/** {block #head} on line 24 */
	public function blockHead(array $ʟ_args): void
	{
		
	}


	/** {block #main} on line 48 */
	public function blockMain(array $ʟ_args): void
	{
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		echo '	<div class="container">
';
		$this->renderBlock('content', [], 'html') /* line 50 */;
		echo '	</div>
';
	}


	/** {snippet flashes} on line 45 */
	public function blockFlashes(array $ʟ_args): void
	{
		extract($this->params);
		extract($ʟ_args);
		unset($ʟ_args);
		$this->global->snippetDriver->enter("flashes", 'static');
		try {
			$this->createTemplate('../../Base/templates/flashMessages.latte', $this->params, 'include')->renderToContentType('html') /* line 46 */;
		} finally {
			$this->global->snippetDriver->leave();
		}
		
	}

}
