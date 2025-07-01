# Plan de refactorisation - Expansion de variables

## Problèmes identifiés :

### 1. **Logique de quotes dupliquée** (4 implémentations différentes)
- `should_expand_at_position()` dans join_str.c
- `is_expandable()` dans var_utils.c  
- `update_quote_status()` dans quotes.c
- `in_sgl()/in_dbl()` dans var_utils.c

### 2. **Détection paramètres positionnels répétée** (3 endroits)
- `is_positional_param()` dans copy_var_content.c
- Logique dans `extract_var_name()` dans find_var.c
- Logique dans `get_name()` dans find_var.c

### 3. **Cas spéciaux dispersés** (3 fonctions)
- `handle_special_cases()` dans copy_var_content.c
- Logique dans `process_next_dollar()` dans join_str.c
- Vérifications dans `find_var()`

### 4. **Double traitement quotes**
- `join_str()` gère les quotes avec `should_expand_at_position()`
- `expand_one_arg()` appelle ensuite `remove_quotes()`

## Solution proposée :

### Étape 1: Centraliser la gestion des quotes
Créer un seul fichier `quote_utils.c` avec :
```c
bool should_expand_at_position(const char *str, int pos);
bool is_in_quotes(const char *str, int pos, char *quote_type);
```

### Étape 2: Centraliser la logique des variables
Regrouper dans `var_utils.c` :
```c
bool is_special_var(const char *name);
bool is_positional_param(const char *name);
char *handle_special_cases(const char *name);
```

### Étape 3: Simplifier le flux principal
```
expand_vars() -> join_str() -> process_next_dollar()
                |
                +-> find_var() + copy_var_content()
```

### Étape 4: Éviter le double traitement
- `join_str()` fait l'expansion ET supprime les quotes
- OU `expand_one_arg()` fait tout en une passe

## Bénéfices :
- ✅ Code plus maintenable
- ✅ Moins de bugs potentiels
- ✅ Logique centralisée
- ✅ Respect Norme 42 (moins de lignes par fonction)
