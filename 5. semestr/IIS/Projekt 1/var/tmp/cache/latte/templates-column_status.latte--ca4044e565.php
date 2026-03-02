<?php

use Latte\Runtime as LR;

/** source: /var/www/html/vendor/ublaboo/datagrid/src/Column/../templates/column_status.latte */
final class Templateca4044e565 extends Latte\Runtime\Template
{

	public function main(): array
	{
		extract($this->params);
		$activeOption = $status->getCurrentOption($row) /* line 6 */;
		echo '
<div class="dropdown">
';
		if ($activeOption) /* line 9 */ {
			if ($status->shouldBeRendered($row)) /* line 10 */ {
				echo '			<button class="dropdown-toggle ';
				echo LR\Filters::escapeHtmlAttr($activeOption->getClass()) /* line 11 */;
				echo ' ';
				echo LR\Filters::escapeHtmlAttr($activeOption->getClassSecondary()) /* line 11 */;
				echo '" type="button" data-toggle="dropdown">
				';
				if ($activeOption->getIcon()) /* line 12 */ {
					echo '<i class="';
					echo LR\Filters::escapeHtmlAttr($iconPrefix) /* line 12 */;
					echo LR\Filters::escapeHtmlAttr($activeOption->getIcon()) /* line 12 */;
					echo '"></i> ';
				}
				echo '
				';
				echo LR\Filters::escapeHtmlText(($this->filters->translate)($activeOption->getText())) /* line 13 */;
				echo ' ';
				if ($status->hasCaret()) /* line 13 */ {
					echo '<i class="caret"></i>
';
				}
				echo '			</button>
';
			} else /* line 15 */ {
				echo '			';
				echo LR\Filters::escapeHtmlText(($this->filters->translate)($activeOption->getText())) /* line 16 */;
				echo "\n";
			}
		} else /* line 18 */ {
			echo '		';
			echo LR\Filters::escapeHtmlText($row->getValue($status->getColumn())) /* line 19 */;
			echo "\n";
		}
		echo '	<ul class="dropdown-menu">
';
		$iterations = 0;
		foreach ($status->getOptions() as $option) /* line 22 */ {
			echo '		<li>
			';
			$confirmationDialog = $option->getConfirmationDialog($row) /* line 23 */;
			echo '

			<a
				class="';
			echo LR\Filters::escapeHtmlAttr($option->getClassInDropdown()) /* line 26 */;
			echo '"
';
			if ($confirmationDialog) /* line 28 */ {
				echo '					data-';
				echo LR\Filters::escapeHtmlAttrUnquoted(Ublaboo\DataGrid\Column\Action::$dataConfirmAttributeName) /* line 29 */;
				echo '=';
				echo LR\Filters::escapeHtmlAttrUnquoted($confirmationDialog) /* line 29 */;
				echo "\n";
			}
			echo '			 href="';
			echo LR\Filters::escapeHtmlAttr($this->global->uiControl->link("changeStatus!", ['id' => $row->getId(), 'key' => $status->getKey(), 'value' => $option->getValue()])) /* line 25 */;
			echo '">
				';
			if ($option->getIconSecondary()) /* line 32 */ {
				echo '<i class="datagrid-column-status-option-icon ';
				echo LR\Filters::escapeHtmlAttr($iconPrefix) /* line 32 */;
				echo LR\Filters::escapeHtmlAttr($option->getIconSecondary()) /* line 32 */;
				echo '"></i> ';
			}
			echo '
				';
			echo LR\Filters::escapeHtmlText(($this->filters->translate)($option->getText())) /* line 33 */;
			echo '
			</a>
		</li>
';
			$iterations++;
		}
		echo '	</ul>
</div>
';
		return get_defined_vars();
	}


	public function prepare(): void
	{
		extract($this->params);
		if (!$this->getReferringTemplate() || $this->getReferenceType() === "extends") {
			foreach (array_intersect_key(['option' => '22'], $this->params) as $ʟ_v => $ʟ_l) {
				trigger_error("Variable \$$ʟ_v overwritten in foreach on line $ʟ_l");
			}
		}
		Nette\Bridges\ApplicationLatte\UIRuntime::initialize($this, $this->parentName, $this->blocks);
		
	}

}
