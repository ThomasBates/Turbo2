using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE
{
    public class BudgetAccount : ITurboBudgetAccount
    {
        [Key]
        public string ID { get; set; }

        public string Caption { get; set; }
        public string Description { get; set; }
        public List<ITurboBudgetTransaction> Transactions { get; set; }
    }
}
