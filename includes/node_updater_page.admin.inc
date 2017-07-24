<?php
/**
 * menu callback
 */
function node_updater_page() {
	$html = '';

	// get 'update_by_node_type_form' form
	// and render it to $html
	$formType = drupal_get_form('update_by_node_type_form');
	$html .= '<fieldset><legend>Update by node type</legend><br/><div class="fieldset-wrapper">'.render($formType).'</div></fieldset>';

	drupal_set_message(t('Warning, database loading may increase during node updating so please be patient.'), 'warning');

	return $html;
}


/**
 * Implements hook_form().
 */
function update_by_node_type_form($form, &$form_state) {
	$wrappername = 'update_by_type';
	$weight = 0;

  $form[$wrappername] = array(
    '#type' => 'container',
    '#prefix' => '<div class="'.$wrappername.'">',
    '#suffix' => '</div>',
  );

  $types = array();
  foreach (node_type_get_types() as $type) { $types[$type->type] = $type->name; }

  if (!empty($types)) {
	  $weight++;
	  $form[$wrappername]['type'] = array(
	  	'#type' => 'checkboxes',
	  	'#title' => t('Select node type to update'),
	  	'#required' => TRUE,
	  	'#options' => $types,
	  );

	  $form[$wrappername]['submit'] = array(
	  	'#type' => 'submit',
	  	'#value' => 'Update',
	  	'#name' => 'op',
	  	'#prefix' => '<div class="form-actions">',
	  	'#suffix' => '</div>',
	  );
  }
  else {
  	$form[$wrappername]['none'] = array(
  		'#type' => 'markup',
  		'#markup' => t('No content types has been added yet.')
  	);
  }

  return $form;
}


/**
 * submit process for update_by_node_type_form
 */
function update_by_node_type_form_submit($form, &$form_state) {
	if (isset($form_state['values'])) {
		$values = $form_state['values'];

		if (isset($values['type'])) {
			$nodetypes = $values['type'];
			// select query to table 'node' based on node types
			// see helper.inc
			$db = nu_db_select_multiple_clause('node', array('nid'), 'type', $nodetypes);
			$nids = array();
			$nodes = array();

			if (!empty($db)) {
				foreach ($db as $value) {
					$nids[] = $value['nid'];
				}

				$nodes = node_load_multiple($nids);
				foreach ($nodes as $node) {
					node_save($node);
				}

				drupal_set_message(t(count($nodes).' nodes found and has been updated.'), 'status');
			}
			else {
				drupal_set_message(t(count($nodes).' node found.'), 'warning');
			}
		}
	}
}