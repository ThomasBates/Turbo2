using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Turbo.Budget.Types
{
    public interface ITurboBudgetTransaction
    {
        int Id { get; set; }

        int Order { get; set; }
        DateTime TimeStamp { get; set; }
        string FromAccountId { get; set; }
        string ToAccountId { get; set; }
        decimal Amount { get; set; }
        string Description { get; set; }

        ITurboBudgetAccount FromAccount { get; set; }
        ITurboBudgetAccount ToAccount { get; set; }
    }
}
